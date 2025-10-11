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
