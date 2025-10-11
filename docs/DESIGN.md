<<<<<<< HEAD
# Scheduler Simulator Design Decisions

## Architecture Overview
The simulator is structured in modular fashion:
- `scheduler.c` drives the main simulation, reading workloads and configuring algorithms.
- Algorithms are isolated in `algorithms.c` for maintainability.
- Metric calculations are centralized in `metrics.c`.
- UI components are in `gui_gtk.c` or `gui_ncurses.c`.

## Process Modeling
Processes are represented by the `process_t` structure, capturing all parameters and computed metrics necessary for scheduling and analysis.

## Timeline Representation
CPU activity is modeled as a series of `timeline_event_t` segments, allowing for Gantt chart visualization and detailed scheduling history.

## Algorithm Selection & Parameterization
Algorithms can be selected and configured interactively via the UI, with real-time adjustments supported for round robin quantum and MLFQ parameters.

## Extensibility
The code base supports easy addition of new algorithms or metrics, and can load process sets from workload files.

## Rationale
A modular design ensures code reusability, easier debugging, and simplified testing, while interactive visualization enhances learning and exploration.
=======
# Design Decisions for msgbuf Kernel Module

## Overview
The msgbuf kernel module implements a character device that acts as a message buffer, supporting reading, writing, and ioctl controls to manage the buffer.

## Design Choices

- **Synchronization:** Mutexes are used to protect access to the buffer to allow sleeping, while spinlocks protect statistical counters for fast updates without sleeping. This ensures safe concurrent access without deadlocks.
- **/proc Interface:** A `/proc/msgbuf_stats` entry is created to expose runtime statistics about the device in a convenient way.
- **IOCTL Commands:** Commands include clearing the buffer, getting current buffer size, and a placeholder for future resizing functionality.
- **Single Source File:** All kernel module code resides in one file (`msgbuf.c`) for easier maintenance and compilation.
- **User-space Testing:** Separate test programs validate basic functionality, concurrency, and stress scenarios.
  
## Future Enhancements

- Implement dynamic buffer resizing support.
- Add support for select/poll and asynchronous notifications.
- Optimize performance and handle additional error cases.
>>>>>>> b65f498 (L2.1-KernelCustom)
