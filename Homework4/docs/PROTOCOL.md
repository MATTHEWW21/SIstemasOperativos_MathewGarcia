# Message Protocol Specification

This document defines the message types and structure used in the chat system.

## Message Types

- MSG_JOIN: Client joining notification
- MSG_LEAVE: Client leaving notification
- MSG_BROADCAST: Message sent to all clients
- MSG_PRIVATE: Private message to a specific client
- MSG_LIST_USERS: Client requests list of users
- MSG_USER_LIST: Server response containing user list
- MSG_ERROR: Error message
- MSG_PING: Keep-alive ping
- MSG_PONG: Keep-alive response

## Message Structure

#define MSG_MAX_SIZE 256
#define USERNAME_MAX 32

typedef enum {
MSG_JOIN,
MSG_LEAVE,
MSG_BROADCAST,
MSG_PRIVATE,
MSG_LIST_USERS,
MSG_USER_LIST,
MSG_ERROR,
MSG_PING,
MSG_PONG
} message_type_t;

typedef struct {
message_type_t type;
char from_user[USERNAME_MAX];
char to_user[USERNAME_MAX];
char text[MSG_MAX_SIZE];
time_t timestamp;
int client_id;
} message_t;


## Message Flow

- Clients send MSG_JOIN on connect, MSG_LEAVE on disconnect
- Broadcast messages use MSG_BROADCAST
- Private messages use MSG_PRIVATE with specified recipient
- Periodic MSG_PING/MSG_PONG maintain connection health
