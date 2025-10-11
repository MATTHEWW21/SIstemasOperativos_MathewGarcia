#include <stdio.h>
#include "../include/scheduler.h"
#include "../include/algorithms.h"
#include "../include/metrics.h"

int main() {
    process_t processes[3] = {
        {1, 0, 6, 1, 6, -1, -1, -1, -1, -1},
        {2, 1, 8, 2, 8, -1, -1, -1, -1, -1},
        {3, 2, 7, 1, 7, -1, -1, -1, -1, -1}
    };
    timeline_event_t timeline[100];
    int n = 3;
    
    schedule_sjf(processes, n, timeline);

    metrics_t metrics;
    calculate_metrics(processes, n, 25, &metrics);

    printf("SJF Test:\nAvg Turnaround: %.2f\nAvg Waiting: %.2f\nAvg Response: %.2f\n\n",
           metrics.avg_turnaround_time, metrics.avg_waiting_time, metrics.avg_response_time);
    return 0;
}
