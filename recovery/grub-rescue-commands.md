# GRUB Rescue Mode Commands

This file lists and explains common commands used in GRUB rescue mode.

- `ls` 
  Lists available devices and partitions.

- `set root=(hdX,Y)` 
  Sets the root partition to GRUB.

- `set prefix=(hdX,Y)/boot/grub` 
  Sets the location of GRUB files.

- `insmod normal` 
  Loads the normal module to move from rescue to normal mode.

- `normal`
  Switches to the normal GRUB boot menu.

- `linux /boot/vmlinuz root=/dev/sdXY`
  Loads the Linux kernel.

- `initrd /boot/initrd.img` 
  Loads the initial ramdisk.

- `boot` 
  Starts the boot process with the loaded kernel.

- `cat filename` 
  Displays the contents of a file (for diagnostic).

- `search --file filename` 
  Searches for a file on the available partitions.

Understanding and using these commands can help recover systems stuck in GRUB rescue mode.
