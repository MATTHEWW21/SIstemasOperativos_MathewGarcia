#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_ZOMBIES 1024

void format_time(long total_seconds, char* buffer, size_t size) {
    int h = total_seconds / 3600;
    int m = (total_seconds % 3600) / 60;
    int s = total_seconds % 60;
    snprintf(buffer, size, "%02d:%02d:%02d", h, m, s);
}

int find_zombies(int *zombie_pids, int max_zombies) {
    DIR *proc = opendir("/proc");
    struct dirent *entry;
    int count = 0;

    if (!proc) {
        perror("opendir /proc");
        return -1;
    }

    while ((entry = readdir(proc)) != NULL) {
        if (!isdigit(entry->d_name[0]))
            continue;

        char stat_path[256];
        snprintf(stat_path, sizeof(stat_path), "/proc/%.200s/stat", entry->d_name);

        FILE *stat_file = fopen(stat_path, "r");
        if (!stat_file)
            continue;

        int pid, ppid;
        char comm[128], state;
        fscanf(stat_file, "%d (%127[^)]) %c %d", &pid, comm, &state, &ppid);
        fclose(stat_file);

        if (state == 'Z' && count < max_zombies) {
            zombie_pids[count++] = pid;
        }
    }
    closedir(proc);
    return count;
}

void print_zombie_info(int pid, int *ppid_out, char *pname_out) {
    char stat_path[256], comm[128], state, comm_name[128], time_buf[16];
    int ppid;
    long utime = 0, stime = 0;
    snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);

    FILE *stat_file = fopen(stat_path, "r");
    if (!stat_file)
        return;
    fscanf(stat_file, "%*d (%127[^)]) %c %d", comm, &state, &ppid);
    fclose(stat_file);

    stat_file = fopen(stat_path, "r");
    if (stat_file) {
        char buffer[2048];
        if (fgets(buffer, sizeof(buffer), stat_file)) {
            char *token = strtok(buffer, " ");
            int field = 1;
            while (token) {
                if (field == 14) utime = atol(token);
                if (field == 15) {
                    stime = atol(token);
                    break;
                }
                token = strtok(NULL, " ");
                field++;
            }
        }
        fclose(stat_file);
    }

    // Get command name from /proc/[pid]/comm
    char comm_path[256];
    snprintf(comm_path, sizeof(comm_path), "/proc/%d/comm", pid);
    FILE *comm_file = fopen(comm_path, "r");
    strcpy(comm_name, "unknown");
    if (comm_file) {
        if (fgets(comm_name, sizeof(comm_name), comm_file)) {
            comm_name[strcspn(comm_name, "\n")] = 0;
        }
        fclose(comm_file);
    }

    // Time formatting
    long total_time = (utime + stime) / sysconf(_SC_CLK_TCK);
    format_time(total_time, time_buf, sizeof(time_buf));

    printf("%-7d %-7d %-15s %-7c %s\n", pid, ppid, comm_name, state, time_buf);

    if (ppid_out) *ppid_out = ppid;
    if (pname_out) strcpy(pname_out, comm_name);
}

int main() {
    int zombie_pids[MAX_ZOMBIES], zombie_ppids[MAX_ZOMBIES], total;
    char zombie_names[MAX_ZOMBIES][128];

    total = find_zombies(zombie_pids, MAX_ZOMBIES);

    printf("=== Zombie Process Report ===\n");
    printf("Total Zombies: %d\n\n", total);
    printf("PID     PPID    Command         State   Time\n");
    printf("-----   -----   -------------   -----   --------\n");

    for (int i = 0; i < total; i++) {
        print_zombie_info(zombie_pids[i], &zombie_ppids[i], zombie_names[i]);
    }

    // Analyze parent processes
    struct {
        int ppid;
        int count;
        char pname[128];
    } parents[MAX_ZOMBIES];
    int parent_unique = 0;

    for (int i = 0; i < total; i++) {
        int found = 0;
        for (int j = 0; j < parent_unique; j++) {
            if (parents[j].ppid == zombie_ppids[i]) {
                parents[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            parents[parent_unique].ppid = zombie_ppids[i];
            parents[parent_unique].count = 1;
            // Get parent process name
            char path[256];
            snprintf(path, sizeof(path), "/proc/%d/comm", zombie_ppids[i]);
            FILE *pfile = fopen(path, "r");
            if (pfile) {
                if (fgets(parents[parent_unique].pname, sizeof(parents[parent_unique].pname), pfile)) {
                    parents[parent_unique].pname[strcspn(parents[parent_unique].pname, "\n")] = 0;
                } else {
                    strcpy(parents[parent_unique].pname, "unknown");
                }
                fclose(pfile);
            } else {
                strcpy(parents[parent_unique].pname, "unknown");
            }
            parent_unique++;
        }
    }

    printf("\nParent Process Analysis:\n");
    for (int i = 0; i < parent_unique; i++) {
        printf("  PID %d (%s) has %d zombie children\n",
               parents[i].ppid, parents[i].pname, parents[i].count);
    }
    return 0;
}
