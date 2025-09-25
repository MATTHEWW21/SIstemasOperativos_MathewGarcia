# Troubleshooting Checklist for GRUB Boot Issues

Use this checklist to diagnose and solve common GRUB boot problems.

- Verify boot partition is correct and accessible.

- Check if `/etc/default/grub` and `/boot/grub/grub.cfg` exist and are not corrupted.

- Validate entries in GRUB menu are correct and free of typos.

- Ensure proper ownership and permissions for GRUB configuration files.

- Confirm that changes are applied by running `sudo update-grub`.

- Backup configuration files regularly before making changes.

- Use a Live USB to mount and repair system partitions when unable to boot.

- Check for specific error messages in GRUB rescue prompt or system logs.

- Reinstall GRUB if configuration files or bootloader are damaged.

- Document changes and recovery procedures for future reference.
