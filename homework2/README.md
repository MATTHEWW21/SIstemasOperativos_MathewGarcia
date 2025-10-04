Zombie Process Handler

Standalone Assignment — Process Lifecycle & Zombie Management
Author: [Your Name]
Course: Operating Systems
Table of Contents

    Project Overview

    Directory Structure

    Build Instructions

    Usage & Test Scripts

    Components Summary

    Library Documentation

    Cleanup

    Demo & Output

    Credits

Project Overview

This project is a modular suite designed to help students understand the lifecycle of processes, the issues and implications of zombie processes in UNIX-like systems, and advanced reaping and prevention strategies.
It includes tools to create, detect, manage, and prevent zombie processes, as well as a reusable static library (libzombie.a) for integration in other C programs.
Directory Structure

homework2/
├── src/
│   ├── zombie_creator.c        # Part 1: Zombie Process Creator
│   ├── zombie_detector.c       # Part 2: Zombie Detector/Reporter
│   ├── zombie_reaper.c         # Part 3: Reaping Strategies
│   ├── process_daemon.c        # Part 4: Long-running Daemon
│   ├── zombie.c                # Part 5: Zombie Prevention Library
│   └── zombie.h                # Part 5: Library Header
├── tests/
│   ├── test_creator.sh
│   ├── test_detector.sh
│   ├── test_reaper.sh
│   └── test_daemon.sh
├── Makefile                    # Build system for all parts
├── README.md                   # Documentation
└── DEMO.md                     # Output/screenshot evidence

Build Instructions

To build all binaries and the library, run:

make

All executables will be placed in src/, and the static library libzombie.a in the project root.
Usage & Test Scripts
Part 1: Zombie Creator

    Compiles to src/zombie_creator

    Usage example:

    ./src/zombie_creator 5

    This creates 5 zombie processes for testing.

Part 2: Zombie Detector

    Compiles to src/zombie_detector

    Usage:

    ./src/zombie_detector

    Scans /proc to report current zombie processes.

Part 3: Zombie Reaper

    Compiles to src/zombie_reaper

    Usage:

    ./src/zombie_reaper 1   # Strategy 1: explicit wait
    ./src/zombie_reaper 2   # Strategy 2: SIGCHLD handler
    ./src/zombie_reaper 3   # Strategy 3: SIGCHLD ignore

Part 4: Daemon

    Compiles to src/process_daemon

    Usage:

    ./src/process_daemon

    This daemonizes itself, launches worker processes periodically, and logs to /tmp/daemon.log.
    Stop using killall process_daemon.

Part 5: Zombie Prevention Library

    Library static archive: libzombie.a, header: src/zombie.h

    Use in your C programs by linking:

    gcc -o myprog myprog.c -L. -lzombie -lpthread

    See the included sample code for usage patterns.

Test Scripts

Quickly verify all functionality using the bash scripts in tests/ (make sure you’ve set them as executable with chmod +x tests/*.sh):

    ./tests/test_creator.sh

    ./tests/test_detector.sh

    ./tests/test_reaper.sh

    ./tests/test_daemon.sh

Components Summary

    zombie_creator.c: Creates user-specified zombie processes for manual inspection and testing.

    zombie_detector.c: Scans and reports all zombie processes, their parent, command, and runtime.

    zombie_reaper.c: Implements three different zombie reaping strategies: explicit wait, SIGCHLD handler, and ignore.

    process_daemon.c: Daemonizes, handles signals, periodically spawns and reaps children, and logs activity without ever leaving zombies.

    zombie.c/h: Library for automatic zombie prevention (thread-safe, statistics support, tested).

Library Documentation

Include #include "zombie.h" in your code.

    void zombie_init(void);
    Initializes zombie prevention (must be called before forking children).

    pid_t zombie_safe_fork(void);
    Safe version of fork that updates zombie statistics.

    int zombie_safe_spawn(const char *command, char *args[]);
    Fork+exec with built-in zombie prevention.

    void zombie_get_stats(zombie_stats_t *stats);
    Obtains statistics about zombies created, reaped, and currently active (thread-safe).

Compile with:

gcc -o myprog myprog.c -L. -lzombie -lpthread

Cleanup

To remove all binaries, objects, and the library, use:

make clean

Demo & Output

Output of test cases and screenshots can be found in DEMO.md. This includes sample runs, process listings, logs, and evidence of proper zombie management for each part.
Credits

Developed for the Operating Systems assignment.
Starter code, documentation, and scripts by Mathew Garcia.
For questions, contact: erickmathewenxv1@gmail.com
