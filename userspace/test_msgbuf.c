#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>

#define DEVICE "/dev/msgbuf"
#define MSGBUF_IOCTL_CLEAR   _IO('M', 1)
#define MSGBUF_IOCTL_GETSIZE _IOR('M', 2, int)

int main() {
    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    
    // Test write
    const char *msg = "Hello from user space!";
    ssize_t written = write(fd, msg, strlen(msg));
    printf("Wrote %zd bytes\n", written);
    
    // Test read
    char buffer[256] = {0};
    lseek(fd, 0, SEEK_SET);  // Reset to beginning
    ssize_t read_bytes = read(fd, buffer, sizeof(buffer));
    printf("Read %zd bytes: %s\n", read_bytes, buffer);
    
    // Test ioctl: get buffer size
    int size = ioctl(fd, MSGBUF_IOCTL_GETSIZE);
    if (size < 0) {
        perror("ioctl getsize");
    } else {
        printf("Current buffer size: %d\n", size);
    }
    
    // Test ioctl: clear buffer
    if (ioctl(fd, MSGBUF_IOCTL_CLEAR) < 0) {
        perror("ioctl clear");
    } else {
        printf("Buffer cleared\n");
    }
    
    close(fd);
    return 0;
}
