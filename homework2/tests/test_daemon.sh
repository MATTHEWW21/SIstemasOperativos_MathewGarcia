#!/bin/bash
echo "Test: process_daemon (should never create zombies)"
../src/process_daemon &
DAEMON_PID=$!
sleep 10
ps aux | grep process_daemon | grep -v grep
echo "Checking for zombies (should be none):"
ps aux | grep defunct | grep -v grep
kill $DAEMON_PID
