#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

volatile sig_atomic_t running = 1;

void sigterm_handler(int sig) {
    running = 0;
}

void sigchld_handler(int sig) {
    // Reap all exited children
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void daemonize(void) {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    if (setsid() < 0) exit(EXIT_FAILURE);

    // Second fork
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    // Set file permissions
    umask(0);

    // Change to root directory
    chdir("/");

    // Close all file descriptors
    for (int fd = 0; fd < 3; fd++) close(fd);

    // Redirect stdin, stdout, stderr to /dev/null
    open("/dev/null", O_RDONLY); // stdin
    open("/dev/null", O_WRONLY); // stdout
    open("/dev/null", O_WRONLY); // stderr
}

void log_msg(const char *msg) {
    FILE *log = fopen("/tmp/daemon.log", "a");
    if (log) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char timebuf[32];
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(log, "[%s] %s\n", timebuf, msg);
        fclose(log);
    }
}

void spawn_workers(void) {
    while (running) {
        pid_t pid = fork();
        if (pid == 0) {
            // Worker: simulate work
            log_msg("Worker started");
            sleep(2 + rand() % 3); // Simulate work (2-4 sec)
            log_msg("Worker finished & exiting");
            exit(0);
        } else if (pid > 0) {
            char buf[64];
            snprintf(buf, sizeof(buf), "Spawned worker with PID %d", pid);
            log_msg(buf);
        }
        sleep(5);
    }
}

int main() {
    // Set up signal handlers before daemonizing
    struct sigaction sa_chld, sa_term;
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa_chld, NULL);

    sa_term.sa_handler = sigterm_handler;
    sigemptyset(&sa_term.sa_mask);
    sa_term.sa_flags = 0;
    sigaction(SIGTERM, &sa_term, NULL);

    daemonize();

    log_msg("Daemon started");
    spawn_workers();
    log_msg("Daemon shutting down gracefully");
    return 0;
}
