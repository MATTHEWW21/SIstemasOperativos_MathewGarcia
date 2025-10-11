#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/slab.h>

#define DEVICE_NAME "msgbuf"
#define BUFFER_SIZE 4096

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tu Nombre");
MODULE_DESCRIPTION("Message Buffer Character Device");
MODULE_VERSION("1.0");

static int major_number;
static struct cdev msgbuf_cdev;
static char device_buffer[BUFFER_SIZE];
static size_t buffer_len = 0;
static struct class *msgbuf_class = NULL;
static struct device *msgbuf_device = NULL;
static DEFINE_MUTEX(buffer_mutex);
static DEFINE_SPINLOCK(stats_lock);

// /proc entry
static struct proc_dir_entry *proc_entry;

// Statistics
static struct {
    unsigned long opens;
    unsigned long reads;
    unsigned long writes;
    unsigned long bytes_read;
    unsigned long bytes_written;
} stats;

// Prototypes
static int msgbuf_open(struct inode *inode, struct file *file);
static int msgbuf_release(struct inode *inode, struct file *file);
static ssize_t msgbuf_read(struct file *file, char __user *user_buf, size_t count, loff_t *offset);
static ssize_t msgbuf_write(struct file *file, const char __user *user_buf, size_t count, loff_t *offset);
static long msgbuf_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#define MSGBUF_IOCTL_CLEAR   _IO('M', 1)
#define MSGBUF_IOCTL_GETSIZE _IOR('M', 2, int)
#define MSGBUF_IOCTL_RESIZE  _IOW('M', 3, int)

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = msgbuf_open,
    .release = msgbuf_release,
    .read = msgbuf_read,
    .write = msgbuf_write,
    .unlocked_ioctl = msgbuf_ioctl,
};

// /proc show function
static int msgbuf_proc_show(struct seq_file *m, void *v) {
    seq_printf(m, "Message Buffer Statistics\n");
    seq_printf(m, "=========================\n");
    seq_printf(m, "Opens:          %lu\n", stats.opens);
    seq_printf(m, "Reads:          %lu\n", stats.reads);
    seq_printf(m, "Writes:         %lu\n", stats.writes);
    seq_printf(m, "Bytes Read:     %lu\n", stats.bytes_read);
    seq_printf(m, "Bytes Written:  %lu\n", stats.bytes_written);
    seq_printf(m, "Buffer Size:    %d\n", BUFFER_SIZE);
    seq_printf(m, "Current Length: %zu\n", buffer_len);
    return 0;
}

// /proc open function
static int msgbuf_proc_open(struct inode *inode, struct file *file) {
    return single_open(file, msgbuf_proc_show, NULL);
}

static const struct proc_ops msgbuf_proc_ops = {
    .proc_open = msgbuf_proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int msgbuf_open(struct inode *inode, struct file *file) {
    if (!mutex_trylock(&buffer_mutex)) {
        printk(KERN_ALERT "msgbuf: Device in use by another process\n");
        return -EBUSY;
    }
    stats.opens++;
    printk(KERN_INFO "msgbuf: Device opened\n");
    return 0;
}

static int msgbuf_release(struct inode *inode, struct file *file) {
    mutex_unlock(&buffer_mutex);
    printk(KERN_INFO "msgbuf: Device closed\n");
    return 0;
}

static ssize_t msgbuf_read(struct file *file, char __user *user_buf, size_t count, loff_t *offset) {
    ssize_t ret;

    if (mutex_lock_interruptible(&buffer_mutex))
        return -ERESTARTSYS;

    if (*offset >= buffer_len) {
        ret = 0;
        goto out;
    }

    size_t to_read = min(count, buffer_len - *offset);
    if (copy_to_user(user_buf, device_buffer + *offset, to_read)) {
        ret = -EFAULT;
        goto out;
    }

    *offset += to_read;
    ret = to_read;

out:
    mutex_unlock(&buffer_mutex);
    spin_lock(&stats_lock);
    stats.reads++;
    if (ret > 0)
        stats.bytes_read += ret;
    spin_unlock(&stats_lock);
    return ret;
}

static ssize_t msgbuf_write(struct file *file, const char __user *user_buf, size_t count, loff_t *offset) {
    ssize_t ret;

    if (mutex_lock_interruptible(&buffer_mutex))
        return -ERESTARTSYS;

    if (count > BUFFER_SIZE)
        count = BUFFER_SIZE;

    if (copy_from_user(device_buffer, user_buf, count)) {
        ret = -EFAULT;
        goto out;
    }

    buffer_len = count;
    device_buffer[buffer_len] = '\0';

    printk(KERN_INFO "msgbuf: Wrote %zu bytes\n", count);
    ret = count;

out:
    mutex_unlock(&buffer_mutex);
    spin_lock(&stats_lock);
    stats.writes++;
    if (ret > 0)
        stats.bytes_written += ret;
    spin_unlock(&stats_lock);
    return ret;
}

static long msgbuf_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
    case MSGBUF_IOCTL_CLEAR:
        mutex_lock(&buffer_mutex);
        buffer_len = 0;
        memset(device_buffer, 0, BUFFER_SIZE);
        mutex_unlock(&buffer_mutex);
        printk(KERN_INFO "msgbuf: Buffer cleared\n");
        return 0;
    case MSGBUF_IOCTL_GETSIZE:
        return buffer_len;
    case MSGBUF_IOCTL_RESIZE:
        return -EINVAL;
    default:
        return -EINVAL;
    }
}

static int __init msgbuf_init(void) {
    dev_t dev;
    int ret;

    ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ALERT "msgbuf: Failed to allocate major number\n");
        return ret;
    }
    major_number = MAJOR(dev);

    cdev_init(&msgbuf_cdev, &fops);
    msgbuf_cdev.owner = THIS_MODULE;

    ret = cdev_add(&msgbuf_cdev, dev, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev, 1);
        printk(KERN_ALERT "msgbuf: Failed to add cdev\n");
        return ret;
    }

    msgbuf_class = class_create(DEVICE_NAME);
    if (IS_ERR(msgbuf_class)) {
        cdev_del(&msgbuf_cdev);
        unregister_chrdev_region(dev, 1);
        printk(KERN_ALERT "msgbuf: Failed to create class\n");
        return PTR_ERR(msgbuf_class);
    }

    msgbuf_device = device_create(msgbuf_class, NULL, dev, NULL, DEVICE_NAME);
    if (IS_ERR(msgbuf_device)) {
        class_destroy(msgbuf_class);
        cdev_del(&msgbuf_cdev);
        unregister_chrdev_region(dev, 1);
        printk(KERN_ALERT "msgbuf: Failed to create device\n");
        return PTR_ERR(msgbuf_device);
    }

    proc_entry = proc_create("msgbuf_stats", 0444, NULL, &msgbuf_proc_ops);
    if (!proc_entry) {
        device_destroy(msgbuf_class, dev);
        class_destroy(msgbuf_class);
        cdev_del(&msgbuf_cdev);
        unregister_chrdev_region(dev, 1);
        printk(KERN_ALERT "msgbuf: Failed to create /proc/msgbuf_stats\n");
        return -ENOMEM;
    }

    mutex_init(&buffer_mutex);
    printk(KERN_INFO "msgbuf: Module loaded with device /dev/%s and major %d\n", DEVICE_NAME, major_number);
    return 0;
}

static void __exit msgbuf_exit(void) {
    proc_remove(proc_entry);
    device_destroy(msgbuf_class, MKDEV(major_number, 0));
    class_unregister(msgbuf_class);
    class_destroy(msgbuf_class);
    cdev_del(&msgbuf_cdev);
    unregister_chrdev_region(MKDEV(major_number, 0), 1);
    mutex_destroy(&buffer_mutex);
    printk(KERN_INFO "msgbuf: Module unloaded\n");
}

module_init(msgbuf_init);
module_exit(msgbuf_exit);
