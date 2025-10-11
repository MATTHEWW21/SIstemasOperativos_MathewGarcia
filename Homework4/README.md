# Chat System Project

This project implements a multi-client chat system in C using IPC mechanisms such as UNIX Domain Sockets and POSIX Message Queues.

## Features

- Server handles multiple clients concurrently.
- Clients can send broadcast and private messages.
- Synchronization with POSIX semaphores to prevent race conditions.
- Message protocol defining various message types and structures.
- Modular code with reusable utilities and organized headers.

## Building

Use the Makefile to compile all components:

## Running

1. Start the server:

./chat_server server


2. Run one or more clients:

./chat_client client


## Testing

Use provided shell scripts in the `tests/` folder to automate connection, messaging, and disconnection scenarios.

## Documentation

- [PROTOCOL.md](docs/PROTOCOL.md) — Message protocol details.
- [ARCHITECTURE.md](docs/ARCHITECTURE.md) — System design and architecture.
- [TESTING.md](docs/TESTING.md) — Test methodology and results.

## License

This project is licensed under the MIT License.

---

Feel free to contribute and report issues.

##author

Erick Mathew Garcia Sanchez
Operating Systems
