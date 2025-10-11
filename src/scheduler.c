#include "scheduler.h"
#include "algorithms.h"
#include "metrics.h"
#include "report.h"
#include <stdio.h>

// Example main routine to demonstrate workflow
int main(int argc, char *argv[]) {
    printf("CPU Scheduler Simulator starting...\n");
    // Load or define processes here, for demo defining simple array:
    process_t processes[3] = {
        {1, 0, 5, 1, 5, -1, -1, -1, -1, -1},
        {2, 1, 3, 2, 3, -1, -1, -1, -1, -1},
        {3, 2, 8, 1, 8, -1, -1, -1, -1, -1}
    };

    timeline_event_t timeline[100];
    int n = 3;

    // Call scheduling algorithm, e.g. FIFO
    schedule_fifo(processes, n, timeline);

    // Calculate metrics and generate report
    metrics_t metrics;
    calculate_metrics(processes, n, /*total_time*/ 30, &metrics);
    generate_report("report.md", processes, n);

    printf("Simulation completed\n");
    return 0;
}
