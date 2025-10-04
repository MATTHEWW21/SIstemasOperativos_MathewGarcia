#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

void reap_explicit(void) {
    int status;
    pid_t pid;

    // Reap all child processes explicitly using waitpid in loop
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Explicitly reaped child PID %d\n", pid);
    }
}

void sigchld_handler(int sig) {
    int saved_errno = errno;
    int status;
    pid_t pid;

    // Reap all zombie children
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("SIGCHLD handler reaped child PID %d\n", pid);
    }

    errno = saved_errno;
}

void setup_auto_reaper(void) {
    struct sigaction sa;

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

void setup_ignore_reaper(void) {
    // Setting SIGCHLD to SIG_IGN causes automatic reaping by the kernel
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <strategy: 1|2|3>\n", argv[0]);
        return 1;
    }

    int strategy = atoi(argv[1]);
    if (strategy < 1 || strategy > 3) {
        fprintf(stderr, "Invalid strategy. Choose 1, 2 or 3.\n");
        return 1;
    }

    // Create 10 children processes
    for (int i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process sleeps random time then exits with code i
            sleep(rand() % 3);
            exit(i);
        }
    }

    // Use chosen strategy to reap zombies
    switch(strategy) {
        case 1: 
            reap_explicit();
            break;
        case 2: 
            setup_auto_reaper();
            break;
        case 3: 
            setup_ignore_reaper();
            break;
    }

    // Sleep to wait for all children to exit and be reaped
    sleep(5);

    // Verify no zombies remain
    printf("Checking for zombie processes:\n");
    system("ps aux | grep '[Z]'");

    return 0;
}
