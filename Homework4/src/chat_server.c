#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <semaphore.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/chat_socket"
#define MAX_CLIENTS 10
#define BUFFER_SIZE 256
#define PORT 8888

typedef struct {
    int fd;
    char name[32];
    int active;
} client_t;

client_t clients[MAX_CLIENTS];
int total_messages = 0;

sem_t *client_list_mutex;
sem_t *message_queue_mutex;

void sem_wait_safe(sem_t *sem) {
    while(sem_wait(sem) == -1 && errno == EINTR) {
        continue;
    }
}

void init_mutexes() {
    client_list_mutex = sem_open("/client_list_mutex", O_CREAT, 0644, 1);
    message_queue_mutex = sem_open("/message_queue_mutex", O_CREAT, 0644, 1);
    if (client_list_mutex == SEM_FAILED || message_queue_mutex == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
}

void cleanup_mutexes() {
    sem_close(client_list_mutex);
    sem_unlink("/client_list_mutex");
    sem_close(message_queue_mutex);
    sem_unlink("/message_queue_mutex");
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

void broadcast_message_safe(const char* sender, const char* message) {
    sem_wait_safe(message_queue_mutex);
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "[MSG] %s -> ALL: %s", sender, message);
    printf("%s", buffer);

    sem_wait_safe(client_list_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].name, sender) != 0) {
            write(clients[i].fd, buffer, strlen(buffer));
        }
    }
    total_messages++;
    sem_post(client_list_mutex);

    sem_post(message_queue_mutex);
}

int find_client_by_fd(int fd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && clients[i].fd == fd)
            return i;
    }
    return -1;
}

int main() {
    int server_fd, client_fd, max_fd, activity;
    struct sockaddr_un addr;
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    memset(clients, 0, sizeof(clients));
    init_mutexes();

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);
    unlink(SOCKET_PATH);
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

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
                if (clients[i].fd > max_fd) max_fd = clients[i].fd;
            }
        }
        sem_post(client_list_mutex);

        activity = select(max_fd +1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            cleanup_mutexes();
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(server_fd, &readfds)) {
            client_fd = accept(server_fd, NULL, NULL);
            if (client_fd < 0) {
                perror("accept failed");
                continue;
            }

            client_t new_client;
            new_client.fd = client_fd;
            strncpy(new_client.name, "unknown", sizeof(new_client.name));
            new_client.active = 1;
            add_client_safe(&new_client);

            printf("[INFO] Client '%s' connected (FD: %d)\n", new_client.name, client_fd);
        }

        sem_wait_safe(client_list_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].active && FD_ISSET(clients[i].fd, &readfds)) {
                int valread = read(clients[i].fd, buffer, BUFFER_SIZE-1);
                if (valread <= 0) {
                    printf("[INFO] Client '%s' disconnected\n", clients[i].name);
                    remove_client_safe(clients[i].fd);

                    int active_users = 0;
                    for (int j = 0; j < MAX_CLIENTS; j++)
                        if (clients[j].active) active_users++;
                    printf("[INFO] Total messages: %d, Active users: %d\n", total_messages, active_users);

                    sem_post(client_list_mutex);
                    continue;
                }
                buffer[valread] = '\0';

                if (strncmp(buffer, "/name ", 6) == 0) {
                    buffer[valread-1] = '\0';
                    strncpy(clients[i].name, buffer+6, sizeof(clients[i].name)-1);
                    printf("[INFO] Client fd %d set name to '%s'\n", clients[i].fd, clients[i].name);
                    sem_post(client_list_mutex);
                    continue;
                }

                sem_post(client_list_mutex);
                broadcast_message_safe(clients[i].name, buffer);
                sem_wait_safe(client_list_mutex);
            }
        }
        sem_post(client_list_mutex);
    }

    cleanup_mutexes();
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
