GRUB Rescue Mode
How to Access Rescue Mode

    Rescue mode starts automatically when GRUB cannot find its configuration files or the boot files are corrupted.

    The prompt looks like:
grub rescue>

You can also manually enter rescue mode by pressing C at the GRUB menu to open the command line.

Basic Commands Available
Command	Description	Example
ls	List drives and partitions	ls / ls (hd0,1)
set	Set or show environment variables	set prefix=(hd0,1)/boot/grub
insmod	Load GRUB modules	insmod normal
normal	Load the normal GRUB menu	normal
boot	Boot the system	boot
cat	Display contents of a file	cat (hd0,1)/boot/grub/grub.cfg
search	Search for files, partitions, labels	search --file /boot/grub/grub.cfg
Manual Boot Process

    Use ls to identify partitions.

    Set root and prefix, e.g.:

set root=(hd0,1)
set prefix=(hd0,1)/boot/grub

Load normal module:

insmod normal
normal

Or boot manually:

    linux /boot/vmlinuz root=/dev/sda1
    initrd /boot/initrd.img
    boot

Recovery from Live USB/DVD
Steps to Mount System Partition and Use chroot

    Boot the computer with a Linux Live USB/DVD.

    Open a terminal in live session.

    Identify system partition with:
lsblk

Mount root partition:

sudo mount /dev/sda1 /mnt

Bind mount essential directories:

sudo mount --bind /dev /mnt/dev
sudo mount --bind /proc /mnt/proc
sudo mount --bind /sys /mnt/sys

Change root:

    sudo chroot /mnt

    Now you are operating inside your installed system environment.

GRUB Reinstallation Process

Within the chroot or your live environment (if chrooted):

    Install GRUB on the boot device (adjust device accordingly):

sudo grub-install /dev/sda

Update GRUB config files:

sudo update-grub

Exit chroot:

exit

Unmount:

sudo umount /mnt/dev /mnt/proc /mnt/sys /mnt

Reboot system:

    sudo reboot

Backup Restoration

    Restore original /etc/default/grub from backup:

sudo cp /etc/default/grub.backup /etc/default/grub

Regenerate grub config:

    sudo update-grub

    Verify system boots correctly again.

Verification Procedures

    Check configuration files content:

cat /etc/default/grub
grep menuentry /boot/grub/grub.cfg

Verify GRUB version:

    grub-install --version

    Confirm menu options and boot time at system startup.

Research sources

    Ubuntu Community Help: GrubHowto

    ArchLinux Wiki: GRUB

    GNU GRUB Official Documentation

    Red Hat Documentation
