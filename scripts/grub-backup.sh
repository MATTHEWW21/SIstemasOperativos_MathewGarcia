#!/bin/bash

BACKUP_DIR="/backup/grub_backup_$(date +%Y%m%d_%H%M%S)"

mkdir -p "$BACKUP_DIR"

FILES=("/etc/default/grub" "/boot/grub/grub.cfg")

for FILE in "${FILES[@]}"; do
  if [ -f "$FILE" ]; then
    cp "$FILE" "$BACKUP_DIR"
    echo "Backed up $FILE to $BACKUP_DIR"
  else
    echo "File $FILE not found, skipping."
  fi
done
