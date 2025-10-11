#!/bin/bash
sudo rmmod msgbuf
sudo rm -f /dev/msgbuf
echo "Module unloaded and device removed."
