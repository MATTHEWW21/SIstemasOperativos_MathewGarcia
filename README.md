<<<<<<< HEAD
# Lab 1.5: Users and Services Management

*Student:* Erick Mathew Garcia Sanchez
*Date:* 29-09-25
*Course:* Operating Systems

## Executive Summary
This lab covered user and group management, service administration with systemd, system monitoring, and automation on Linux. Users and groups were created, permissions and sudoers were configured, services like Apache2 and a custom service were managed, and troubleshooting and performance analysis were documented.

## Tasks Completed
- User and group management
- Service configuration and control
- System monitoring and log analysis
- Custom service creation
- Security configuration

## Users and Groups Created
| User | Groups | Home Directory | Shell | Purpose |
|------|--------|----------------|-------|---------|
| alice | developers, administrators | /home/alice | /bin/bash | Lead Developer |
| bob | developers | /home/bob | /bin/bash | Junior Developer |
| charlie | testers | /home/charlie | /bin/bash | QA Tester |

## Services Configured
- Apache2 web server
- Custom simple-service
- SSH service analysis
- System service dependencies

## Key Learnings


    Learned to manage users, groups, and permissions.

    Practiced controlling and troubleshooting services with systemd.

    Automated admin tasks with shell scripts.

    The most challenging part was diagnosing service errors using logs.

    Best practices: document changes, use secure passwords, and apply least privilege.


## Security Measures Implemented


    Used strong, unique passwords for all users.

    Set restrictive permissions on shared directories.

    Limited sudo access and created custom sudoers rules.

    Backed up configurations before making changes.

=======
# CPU Scheduler Simulator

## Overview
This project is a CPU Scheduler Simulator with both graphical (GTK) and terminal (ncurses) interfaces.  
It allows users to simulate and compare major CPU scheduling algorithms, visualize process execution with Gantt charts, and analyze essential scheduling metrics.

## Features
- Simulates FIFO, SJF, STCF, Round Robin, and MLFQ algorithms
- Workloads can be loaded from file
- Gantt chart and metrics dashboard
- Interactive GUI (GTK) and terminal UI (ncurses)
- Generates performance reports

## Project Structure
- `src/` : source code modules (simulation, algorithms, metrics, gui, reports)
- `include/` : header files
- `tests/` : standalone test programs for testing algorithms and metrics
- `workloads/` : example input files for process sets
- `docs/` : design, algorithm, and analysis documentation

## Requirements
- GCC
- GTK+3 for GUI (`sudo apt install libgtk-3-dev`)
- Ncurses for terminal UI (`sudo apt install ncurses-dev`)

## Compiling

To build everything:

make
To build and run the GTK GUI version:

make scheduler_gtk
./scheduler_gtk

To build and run the ncurses terminal version:

make scheduler_ncurses
./scheduler_ncurses


To run all algorithm tests:

make tests
./test_fifo
./test_sjf
./test_stcf
./test_rr
./test_mlfq


To clean all compiled binaries:

make clean


## Usage
Run either interface, select your algorithm, load or input a workload, and view the timeline plus algorithm metrics.  
See `docs/` for sample analysis and explanations.

## Author
Erick Mathew Garcia Sanchez
Operating Systems
>>>>>>> d7f54e5 (L2.4-Scheduler Simulator GUI)
