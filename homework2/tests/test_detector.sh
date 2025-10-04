#!/bin/bash
echo "Test: zombie_detector (detects zombies)"
../src/zombie_creator 2 &
CREATOR_PID=$!
sleep 1
echo "Running detector:"
../src/zombie_detector
kill $CREATOR_PID
