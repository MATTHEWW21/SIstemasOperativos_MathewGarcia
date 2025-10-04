#include <stdio.h>
#include <unistd.h>
#include "zombie.h"

int main() {
    zombie_init();

    for (int i = 0; i < 5; i++) {
        pid_t pid = zombie_safe_fork();
        if (pid == 0) {
            // Child
            sleep(1);
            _exit(0);
        }
    }

    sleep(2); // Wait for children to finish

    zombie_stats_t stats;
    zombie_get_stats(&stats);
    printf("Created: %d, Reaped: %d, Active: %d\n",
        stats.zombies_created, stats.zombies_reaped, stats.zombies_active);
    return 0;
}
