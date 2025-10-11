#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define DEVICE "/dev/msgbuf"

int main() {
    for (int i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            // Proceso hijo escribe repetidamente
            int fd = open(DEVICE, O_WRONLY);
            if (fd < 0) {
                perror("open");
                exit(1);
            }
            for (int j = 0; j < 100; j++) {
                char buf[64];
                snprintf(buf, sizeof(buf), "Process %d, iteration %d\n", getpid(), j);
                ssize_t ret = write(fd, buf, strlen(buf));
                if (ret < 0) {
                    perror("write");
                    close(fd);
                    exit(1);
                }
                usleep(1000);  // 1 ms pausa
            }
            close(fd);
            exit(0);
        }
    }
    // Proceso padre espera a todos los hijos
    for (int i = 0; i < 10; i++) {
        wait(NULL);
    }
    printf("Concurrent writing test completed.\n");
    return 0;
}
