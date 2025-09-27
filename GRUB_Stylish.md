How to Install Stylish Theme for GRUB on Ubuntu VM
Prerequisites

Make sure you have git installed:

bash
sudo apt update
sudo apt install git

Step 1: Clone the grub2-themes Repository

Clone the official repository from GitHub:

bash
git clone https://github.com/vinceliuice/grub2-themes.git
cd grub2-themes

Step 2: Install the Stylish Theme

Run the installation script for the Stylish theme. You can specify screen resolution as needed.

bash
# Basic Stylish installation:
sudo ./install.sh -t stylish

# Specify resolution (example: Full HD 1080p):
sudo ./install.sh -t stylish -s 1080p

# Install directly to /boot/grub/themes:
sudo ./install.sh -b -t stylish

If you simply run:

bash
sudo ./install.sh

A user interface will help you select themes and options interactively.
Step 3: Update GRUB Configuration

Regardless of installation method, update GRUB to apply the theme:

bash
sudo update-grub

or (if your system uses grub-mkconfig):

bash
sudo grub-mkconfig -o /boot/grub/grub.cfg

Step 4: Reboot and Enjoy

Restart your VM to see the new Stylish theme applied:

bash
reboot

Uninstalling the Theme

To remove the theme:

bash
sudo ./install.sh -r -t stylish
sudo update-grub
