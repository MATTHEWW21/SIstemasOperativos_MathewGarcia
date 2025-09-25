#!/bin/bash

echo "=== System Information ==="
uname -a
echo

echo "=== GRUB Version ==="
grub-install --version
echo

echo "=== Current grub config ==="
cat /etc/default/grub
echo

echo "=== Boot menu entries ==="
grep menuentry /boot/grub/grub.cfg
echo
