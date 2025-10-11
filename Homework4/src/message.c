#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#define MSG_MAX_SIZE 256
#define USERNAME_MAX 32
#define SOCKET_PATH "/tmp/chat_socket"
#define MAX_CLIENTS 10
#define PORT 8888

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

typedef struct {
    int fd;
    char name[USERNAME_MAX];
    int active;
} client_t;

client_t clients[MAX_CLIENTS];
int total_messages = 0;

sem_t *client_list_mutex;

void sem_wait_safe(sem_t *sem) {
    while(sem_wait(sem) == -1 && errno == EINTR) {
        continue;
    }
}

void init_mutexes() {
    client_list_mutex = sem_open("/client_list_mutex", O_CREAT, 0644, 1);
    if (client_list_mutex == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
}

void cleanup_mutexes() {
    sem_close(client_list_mutex);
    sem_unlink("/client_list_mutex");
}

void add_client_safe(client_t *client) {
    sem_wait_safe(client_list_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i] = *client;
            clients[i].active = 1;
            break;
        }
    }
    sem_post(client_list_mutex);
}

void remove_client_safe(int fd) {
    sem_wait_safe(client_list_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && clients[i].fd == fd) {
            clients[i].active = 0;
            close(clients[i].fd);
            break;
        }
    }
    sem_post(client_list_mutex);
}

int find_client_by_name(const char *name) {
    for (int i = 0; i < MAX_CLIENTS; i++)
        if (clients[i].active && strcmp(clients[i].name, name) == 0)
            return i;
    return -1;
}

int find_client_by_fd(int fd) {
    for (int i = 0; i < MAX_CLIENTS; i++)
        if (clients[i].active && clients[i].fd == fd)
            return i;
    return -1;
}

void broadcast_message_safe(message_t *msg) {
    sem_wait_safe(client_list_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            write(clients[i].fd, msg, sizeof(message_t));
        }
    }
    sem_post(client_list_mutex);
    total_messages++;
    printf("[MSG] %s -> ALL: %s\n", msg->from_user, msg->text);
}

void private_message_safe(message_t *msg) {
    sem_wait_safe(client_list_mutex);
    int idx = find_client_by_name(msg->to_user);
    if (idx != -1 && clients[idx].active) {
        write(clients[idx].fd, msg, sizeof(message_t));
        printf("[MSG] %s -> %s: %s\n", msg->from_user, msg->to_user, msg->text);
        total_messages++;
    }
    sem_post(client_list_mutex);
}

int count_active_clients() {
    int act = 0;
    sem_wait_safe(client_list_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) act++;
    }
    sem_post(client_list_mutex);
    return act;
}

int main() {
    int server_fd, client_fd, max_fd, activity;
    struct sockaddr_un addr;
    fd_set readfds;
    message_t msg;

    memset(clients, 0, sizeof(clients));
    init_mutexes();

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);
    unlink(SOCKET_PATH);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, MAX_CLIENTS);

    printf("[SERVER] Chat server started on port %d\n", PORT);
    printf("[SERVER] Using IPC method: UNIX_SOCKETS\n");

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_fd = server_fd;

        sem_wait_safe(client_list_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].active) {
                FD_SET(clients[i].fd, &readfds);
                if (clients[i].fd > max_fd)
                    max_fd = clients[i].fd;
            }
        }
        sem_post(client_list_mutex);

        activity = select(max_fd+1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            break;
        }

        if (FD_ISSET(server_fd, &readfds)) {
            client_fd = accept(server_fd, NULL, NULL);
            client_t new_client;
            new_client.fd = client_fd;
            strncpy(new_client.name, "unknown", USERNAME_MAX);
            new_client.active = 1;
            add_client_safe(&new_client);
            printf("[INFO] Client '%s' connected (FD: %d)\n", new_client.name, client_fd);
        }

        sem_wait_safe(client_list_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].active && FD_ISSET(clients[i].fd, &readfds)) {
                int valread = read(clients[i].fd, &msg, sizeof(message_t));
                if (valread <= 0) {
                    printf("[INFO] Client '%s' disconnected\n", clients[i].name);
                    remove_client_safe(clients[i].fd);
                    int active_users = count_active_clients();
                    printf("[INFO] Total messages: %d, Active users: %d\n", total_messages, active_users);
                    continue;
                }
                switch(msg.type) {
                    case MSG_BROADCAST:
                        broadcast_message_safe(&msg); break;
                    case MSG_PRIVATE:
                        private_message_safe(&msg); break;
                    default:
                        break;
                }
            }
        }
        sem_post(client_list_mutex);
    }

    cleanup_mutexes();
    close(server_fd); unlink(SOCKET_PATH);
    return 0;
}
