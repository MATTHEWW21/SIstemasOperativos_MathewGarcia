# Entrada de Arranque Personalizada en GRUB

Se creó una entrada personalizada en GRUB para arrancar el kernel 6.14.0-29-generic en la partición raíz /dev/sda2.

## Pasos realizados

1. Crear archivo `/etc/grub.d/40_custom_personalizado` con el siguiente contenido:

#!/bin/sh
exec tail -n +3 $0

menuentry "Linux personalizado (6.14.0-29-generic)" {
set root=(hd0,2)
linux /boot/vmlinuz-6.14.0-29-generic root=/dev/sda2 quiet splash
initrd /boot/initrd.img-6.14.0-29-generic
}


2. Dar permisos de ejecución:

sudo chmod +x /etc/grub.d/40_custom_personalizado

3. Actualizar GRUB:

sudo update-grub

4. Reiniciar y seleccionar la opción “Linux personalizado (6.14.0-29-generic)” en el menú de GRUB.
