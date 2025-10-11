#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct {
    int pid;                    // Process ID
    int arrival_time;           // When process arrives
    int burst_time;             // Total CPU time needed
    int priority;               // Priority (lower = higher priority)
    int remaining_time;         // Time left to execute
    int start_time;             // First time scheduled
    int completion_time;        // When finished
    int turnaround_time;        // completion_time - arrival_time
    int waiting_time;           // turnaround_time - burst_time
    int response_time;          // start_time - arrival_time
} process_t;

typedef struct {
    int time;                   // Time slice start
    int pid;                    // Process running
    int duration;               // How long it ran
} timeline_event_t;

void schedule_fifo(process_t *processes, int n, timeline_event_t *timeline);

void schedule_sjf(process_t *processes, int n, timeline_event_t *timeline);

void schedule_stcf(process_t *processes, int n, timeline_event_t *timeline);

void schedule_rr(process_t *processes, int n, int quantum, timeline_event_t *timeline);

typedef struct {
    int num_queues;
    int *quantums;              // Quantum for each queue
    int boost_interval;         // Priority boost interval
} mlfq_config_t;

void schedule_mlfq(process_t *processes, int n, mlfq_config_t *config, timeline_event_t *timeline);

#endif // SCHEDULER_H
