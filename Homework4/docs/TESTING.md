# Test Results and Methodology

## Test Setup

- Server and clients run on Ubuntu 20.04 VM.
- Compiled with GCC using pthread and realtime extensions (-lpthread -lrt).
- Used shell scripts to automate connection, messaging, and disconnection scenarios.

## Tests Conducted

1. **Basic Connectivity**
   - Server starts.
   - Multiple clients connect and authenticate username.
   - Clients send broadcast messages successfully.
   - Log outputs show proper message routing.

2. **Private Messaging**
   - Clients send private messages using `/msg` command.
   - Server routes messages correctly and logs events.
   - Recipients receive private messages as expected.

3. **Concurrency and Synchronization**
   - Simultaneous messages from multiple clients handled without errors.
   - Semaphores prevent race conditions during client list modifications.
   - No deadlocks observed in stress tests.

4. **Disconnection Handling**
   - Clients disconnect and server logs updated.
   - User lists update correctly.
   - Resources released properly with no leaks.

## Summary

All core requirements met with stable and expected behavior. Future improvements include:

- Enhanced error recovery.
- Persistent chat history.
- Secure authentication.

