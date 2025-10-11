#include <stdio.h>
#include "../include/scheduler.h"
#include "../include/algorithms.h"
#include "../include/metrics.h"
#include <stdlib.h>

int main() {
    process_t processes[3] = {
        {1, 0, 10, 1, 10, -1, -1, -1, -1, -1},
        {2, 2, 5, 2, 5, -1, -1, -1, -1, -1},
        {3, 3, 8, 1, 8, -1, -1, -1, -1, -1}
    };
    timeline_event_t timeline[100];
    int n = 3;

    int quantums_array[] = {4, 8, 12};
    mlfq_config_t config = {3, quantums_array, 20};

    schedule_mlfq(processes, n, &config, timeline);

    metrics_t metrics;
    calculate_metrics(processes, n, 40, &metrics);

    printf("MLFQ Test:\nAvg Turnaround: %.2f\nAvg Waiting: %.2f\nAvg Response: %.2f\n\n",
           metrics.avg_turnaround_time, metrics.avg_waiting_time, metrics.avg_response_time);
    return 0;
}
