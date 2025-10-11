#!/bin/bash
# Start multiple clients

N=${1:-3} # Number of clients, default 3

for i in $(seq 1 $N); do
    gnome-terminal -- bash -c "../src/chat_client; exec bash" &
    sleep 0.5
done

echo "$N clients started!"
