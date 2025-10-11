# Chat System Architecture

## Overall Design

The chat system follows a client-server model. Clients connect to a central server that manages message routing, user sessions, and concurrency control.

## Components

- Server:
  - Listens for incoming client connections over UNIX Domain Sockets.
  - Maintains list of connected clients and usernames.
  - Routes messages (broadcast/private) to appropriate clients.
  - Uses POSIX semaphores for synchronization to prevent race conditions.
  - Supports message queue IPC for alternative communication channel.

- Client:
  - Connects to server over UNIX Domain Sockets.
  - Sends/receives messages using standard message protocol.
  - Implements commands such as /help, /quit, /msg for communication control.

- IPC Mechanisms:
  - UNIX Domain Sockets: Main communication method for real-time interaction.
  - POSIX Message Queues: Used for messaging control and optional async notifications.

## Synchronization

- Semaphores protect shared client lists and message variables.
- Ensures thread-safe access and proper ordering of operations.
- Prevents deadlocks and race conditions.

## Scalability and Extensibility

- Designed to support multiple clients simultaneously.
- Modular protocol allows introduction of new message types or commands.
- Clean separation between IPC, protocol, server, and client logic.

