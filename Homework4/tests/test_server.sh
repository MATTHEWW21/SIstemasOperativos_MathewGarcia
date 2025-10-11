#!/bin/bash
# Start the chat server

echo "Starting server..."
../src/chat_server &
SERVER_PID=$!
echo "Server running with PID $SERVER_PID"
# Optional: Wait for user to press a key to stop
read -p "Press Enter to stop server..."
kill $SERVER_PID
