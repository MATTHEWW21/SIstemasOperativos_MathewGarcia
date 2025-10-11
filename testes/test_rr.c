#include <stdio.h>
#include "../include/scheduler.h"
#include "../include/algorithms.h"
#include "../include/metrics.h"

int main() {
    process_t processes[3] = {
        {1, 0, 5, 1, 5, -1, -1, -1, -1, -1},
        {2, 1, 3, 2, 3, -1, -1, -1, -1, -1},
        {3, 2, 8, 1, 8, -1, -1, -1, -1, -1}
    };
    timeline_event_t timeline[100];
    int n = 3;
    int quantum = 3;
    
    schedule_rr(processes, n, quantum, timeline);

    metrics_t metrics;
    calculate_metrics(processes, n, 30, &metrics);

    printf("Round Robin Test (Quantum %d):\nAvg Turnaround: %.2f\nAvg Waiting: %.2f\nAvg Response: %.2f\n\n",
           quantum, metrics.avg_turnaround_time, metrics.avg_waiting_time, metrics.avg_response_time);
    return 0;
}
