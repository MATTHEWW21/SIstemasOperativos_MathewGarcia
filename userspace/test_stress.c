#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define DEVICE "/dev/msgbuf"

int main() {
    pid_t pid;
    for (int i = 1; i <= 100; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Proceso hijo escribe un mensaje simple
            int fd = open(DEVICE, O_WRONLY);
            if (fd < 0) {
                perror("open");
                exit(1);
            }
            char msg[64];
            snprintf(msg, sizeof(msg), "Process %d\n", i);
            if (write(fd, msg, strlen(msg)) < 0) {
                perror("write");
                close(fd);
                exit(1);
            }
            close(fd);
            exit(0);
        }
    }

    // Proceso padre espera a todos los hijos
    for (int i = 1; i <= 100; i++) {
        wait(NULL);
    }

    // Comando para buscar errores en dmesg
    printf("Buscando errores en logs del kernel...\n");
    system("dmesg | grep -i error");

    return 0;
}
