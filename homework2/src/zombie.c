#include "zombie.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <errno.h>


static zombie_stats_t stats = {0, 0, 0};
static pthread_mutex_t stats_mutex = PTHREAD_MUTEX_INITIALIZER;

// SIGCHLD handler for reaping children
static void sigchld_handler(int sig) {
    int saved_errno = errno;
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        pthread_mutex_lock(&stats_mutex);
        stats.zombies_reaped++;
        stats.zombies_active--;
        pthread_mutex_unlock(&stats_mutex);
    }
    errno = saved_errno;
}

void zombie_init(void) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDSTOP | SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);
}

pid_t zombie_safe_fork(void) {
    pid_t pid = fork();
    pthread_mutex_lock(&stats_mutex);
    if (pid == 0) {
        // In child, nothing to update
        pthread_mutex_unlock(&stats_mutex);
        return 0;
    }
    if (pid > 0) {
        stats.zombies_created++;
        stats.zombies_active++;
    }
    pthread_mutex_unlock(&stats_mutex);
    return pid;
}

int zombie_safe_spawn(const char *command, char *args[]) {
    pid_t pid = zombie_safe_fork();
    if (pid == 0) {
        execvp(command, args);
        _exit(127);
    }
    return pid;
}

void zombie_get_stats(zombie_stats_t *out) {
    pthread_mutex_lock(&stats_mutex);
    *out = stats;
    pthread_mutex_unlock(&stats_mutex);
}
