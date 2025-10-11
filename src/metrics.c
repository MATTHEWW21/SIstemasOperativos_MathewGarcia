#include "metrics.h"
#include <stdio.h>

void calculate_metrics(process_t *processes, int n, int total_time, metrics_t *metrics) {
    double total_turnaround = 0, total_waiting = 0, total_response = 0;
    int completed = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].completion_time >= 0) {
            processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
            processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
            processes[i].response_time = processes[i].start_time - processes[i].arrival_time;
            total_turnaround += processes[i].turnaround_time;
            total_waiting += processes[i].waiting_time;
            total_response += processes[i].response_time;
            completed++;
        }
    }
    if (completed == 0) completed = 1;
    metrics->avg_turnaround_time = total_turnaround / completed;
    metrics->avg_waiting_time = total_waiting / completed;
    metrics->avg_response_time = total_response / completed;
    metrics->cpu_utilization = ((double)(total_time - 0) / total_time) * 100;  // Simplified
    metrics->throughput = (double)completed / total_time;
    // Jain's fairness index simplified for demo:
    double sum_xi = total_turnaround;
    double sum_xi2 = 0;
    for (int i = 0; i < n; i++) {
        double val = processes[i].turnaround_time;
        sum_xi2 += val * val;
    }
    metrics->fairness_index = (sum_xi * sum_xi) / (completed * sum_xi2);
}
