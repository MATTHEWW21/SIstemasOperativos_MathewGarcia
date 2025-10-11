#!/bin/bash
sudo insmod ../module/msgbuf.ko
sleep 1
if [ ! -e /dev/msgbuf ]; then
    sudo mknod /dev/msgbuf c $(awk '$2=="msgbuf" {print $1}' /proc/devices) 0
fi
echo "Module loaded and device created."
