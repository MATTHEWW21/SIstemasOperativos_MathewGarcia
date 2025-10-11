#!/bin/bash
# Test clients connecting and disconnecting

echo "Starting server..."
../src/chat_server &
SERVER_PID=$!
sleep 1

echo "Connecting client 1..."
../src/chat_client < /dev/null &
C1=$!
sleep 2

echo "Connecting client 2..."
../src/chat_client < /dev/null &
C2=$!
sleep 2

echo "Disconnecting client 1 and 2"
kill $C1
kill $C2

sleep 1
echo "Shutting down server"
kill $SERVER_PID
