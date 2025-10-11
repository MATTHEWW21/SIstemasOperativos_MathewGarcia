#include "scheduler.h"
#include <stdio.h>

// FIFO Scheduling - non-preemptive
void schedule_fifo(process_t *processes, int n, timeline_event_t *timeline) {
    int current_time = 0;
    int index = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time)
            current_time = processes[i].arrival_time;
        timeline[index].time = current_time;
        timeline[index].pid = processes[i].pid;
        timeline[index].duration = processes[i].burst_time;
        processes[i].start_time = current_time;
        current_time += processes[i].burst_time;
        processes[i].completion_time = current_time;
        index++;
    }
}

// Placeholder stubs for other algorithms
void schedule_sjf(process_t *processes, int n, timeline_event_t *timeline) {
    printf("SJF scheduling not implemented yet\n");
}

void schedule_stcf(process_t *processes, int n, timeline_event_t *timeline) {
    printf("STCF scheduling not implemented yet\n");
}

void schedule_rr(process_t *processes, int n, int quantum, timeline_event_t *timeline) {
    printf("Round Robin scheduling not implemented yet\n");
}

void schedule_mlfq(process_t *processes, int n, mlfq_config_t *config, timeline_event_t *timeline) {
    printf("MLFQ scheduling not implemented yet\n");
}
