# Recovery Procedures

This document describes step-by-step procedures to recover your system if GRUB fails to boot.

## Method 1: Boot from Live USB/DVD and Repair

1. Insert a Live USB or DVD with Linux and boot from it.
2. Open a terminal in the live environment.
3. Identify the root partition of your installed system:

lsblk


4. Mount the root partition (replace `/dev/sda1` with your partition):

sudo mount /dev/sda1 /mnt


5. Bind system directories:

sudo mount --bind /dev /mnt/dev
sudo mount --bind /proc /mnt/proc
sudo mount --bind /sys /mnt/sys


6. Change root into the installed system:

sudo chroot /mnt


7. Reinstall GRUB bootloader (replace `/dev/sda` with your disk):

grub-install /dev/sda


8. Regenerate GRUB configuration:

update-grub


9. Exit chroot and unmount:

exit
sudo umount /mnt/dev /mnt/proc /mnt/sys /mnt


10. Reboot the system:

 ```
 reboot
 ```

---

## Method 2: GRUB Rescue Mode (Minimal Commands)

When the system drops to `grub rescue>` prompt, you can try to boot manually.

1. List devices and partitions:

ls


2. Find your boot partition. For example:

ls (hd0,1)/


3. Set root and prefix variables:

set root=(hd0,1)
set prefix=(hd0,1)/boot/grub


4. Load the normal module and boot menu:

insmod normal
normal


5. If normal fails, try to boot manually:

linux /boot/vmlinuz root=/dev/sda1
initrd /boot/initrd.img
boot

---

## Method 3: Restore from Backups

If you have backups of GRUB config files:

1. Copy backup configurations back to original locations, e.g.:

sudo cp /path/to/backup/grub /etc/default/grub
sudo cp /path/to/backup/grub.cfg /boot/grub/grub.cfg



2. Update the GRUB configuration:

sudo update-grub


3. Reboot and verify.

---

These procedures help recover from common boot failures related to GRUB.
