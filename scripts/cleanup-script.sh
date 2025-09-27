#!/bin/bash
# Cleanup script for lab users, groups, and service

for user in alice bob charlie temp_user; do
  if id "$user" &>/dev/null; then
    sudo userdel -r "$user"
    echo "User $user deleted."
  fi
done

for group in developers testers administrators; do
  if getent group "$group" >/dev/null; then
    sudo groupdel "$group"
    echo "Group $group deleted."
  fi
done

if systemctl is-active --quiet simple-service; then
  sudo systemctl stop simple-service
fi
sudo systemctl disable simple-service
sudo rm -f /etc/systemd/system/simple-service.service
sudo systemctl daemon-reload
