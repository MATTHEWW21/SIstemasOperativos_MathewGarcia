#!/bin/bash
echo "Test: zombie_creator (creates zombies)"
../src/zombie_creator 3 &
CREATOR_PID=$!

sleep 2
echo "Checking for zombies..."
ps aux | grep '[Z]' | grep -v grep
kill $CREATOR_PID
