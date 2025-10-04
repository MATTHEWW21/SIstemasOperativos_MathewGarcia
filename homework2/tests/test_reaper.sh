#!/bin/bash
echo "Test: zombie_reaper strategies"
for i in 1 2 3; do
  echo "Strategy $i:"
  ../src/zombie_reaper $i
done

