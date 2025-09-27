#!/bin/bash
# User and group creation script with error handling
set -e

# Define users and their groups
declare -A user_groups=(
  [alice]="developers administrators"
  [bob]="developers"
  [charlie]="testers"
)

# Create groups
for group in developers testers administrators; do
  if ! getent group "$group" >/dev/null; then
    sudo groupadd "$group"
    echo "Group $group created."
  else
    echo "Group $group already exists."
  fi
done

# Create users and add to groups
for user in "${!user_groups[@]}"; do
  if id "$user" &>/dev/null; then
    echo "User $user already exists."
  else
    sudo useradd -m -s /bin/bash "$user"
    echo "User $user created."
    # Set a temporary password (change after first login)
    echo "$user:TempPass123!" | sudo chpasswd
  fi
  # Add user to groups
  for group in ${user_groups[$user]}; do
    sudo usermod -aG "$group" "$user"
    echo "Added $user to $group."
  done
done
