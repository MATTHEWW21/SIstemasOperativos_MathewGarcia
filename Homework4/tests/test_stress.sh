#!/bin/bash
# Stress test: Start 20 clients rapidly

CLIENTS=20
for i in $(seq 1 $CLIENTS); do
    ../src/chat_client < /dev/null &
    echo "Client $i started"
    sleep 0.1
done

echo "Stress test: $CLIENTS clients started."
