# CPU Scheduling Algorithms Explained

## FIFO (First In First Out)
- Processes execute in their arrival order.
- Non-preemptive—once a process starts, it runs until completion.
- Simple, but may lead to long wait times for later arrivals.

## Shortest Job First (SJF)
- Chooses the arrived process with the shortest burst time.
- Non-preemptive; aims to minimize average waiting time.
- Can result in starvation for long jobs.

## Shortest Time to Completion First (STCF)
- Preemptive version of SJF.
- Always selects the process with the shortest remaining time.
- Provides optimal average turnaround time.

## Round Robin (RR)
- Processes are cycled in order, each receiving a fixed time quantum.
- Preemptive and fair; quantum size impacts responsiveness and overhead.

## Multi-Level Feedback Queue (MLFQ)
- Organizes processes into multiple priority queues.
- Processes can be promoted/demoted based on CPU usage.
- Aims to balance throughput with responsiveness for interactive and batch jobs.

## Metrics Used
- Turnaround time, waiting time, response time, utilization, throughput, fairness—explained in formulas.
