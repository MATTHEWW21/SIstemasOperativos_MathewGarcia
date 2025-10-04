#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int create_zombies(int count) {
    for (int i = 0; i < count; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return -1;
        } else if (pid == 0) {

            exit(i);
        } else {

            printf("Created zombie: PID %d (exit code %d)\n", pid, i);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Use: %s <number_of_zombie>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "The number of zombies must be greater than 0.\n");
        return 1;
    }

    if (create_zombies(n) != 0) {
        fprintf(stderr, "Error creating zombie process.\n");
        return 1;
    }

    printf("Press Enter to exit and clean up zombies...\n");
    getchar();

    return 0;
}
