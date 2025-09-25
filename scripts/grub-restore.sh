#!/bin/bash

RESTORE_DIR="/backup/grub_backup_latest"

if [ ! -d "$RESTORE_DIR" ]; then
  echo "Restore directory $RESTORE_DIR does not exist!"
  exit 1
fi

FILES=("grub" "grub.cfg")

for FILE in "${FILES[@]}"; do
  SRC="$RESTORE_DIR/$FILE"
  DST=""
  if [ "$FILE" == "grub" ]; then
    DST="/etc/default/grub"
  else
    DST="/boot/grub/grub.cfg"
  fi

  if [ -f "$SRC" ]; then
    cp "$SRC" "$DST"
    echo "Restored $DST from $SRC"
  else
    echo "Backup file $SRC not found, skipping."
  fi
done
