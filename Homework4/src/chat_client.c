#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>

#define SOCKET_PATH "/tmp/chat_socket"
#define BUFFER_SIZE 256

void print_help() {
    printf("/help               - Show supported commands\n");
    printf("/users              - List online users\n");
    printf("/msg <user> <msg>   - Send private message\n");
    printf("/quit               - Disconnect from server\n");
}

int main() {
    int sockfd;
    struct sockaddr_un address;
    fd_set read_fds;
    char buffer[BUFFER_SIZE], username[32];

    printf("=== Chat Client ===\n");
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    // Create client socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server!\n");
    snprintf(buffer, sizeof(buffer), "/username %s\n", username);
    send(sockfd, buffer, strlen(buffer), 0);

    printf("[SYSTEM] Welcome %s!\n", username);

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);      // stdin (keyboard)
        FD_SET(sockfd, &read_fds); // server socket

        int max_fd = sockfd;

        select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        // User input available
        if (FD_ISSET(0, &read_fds)) {
            fgets(buffer, sizeof(buffer), stdin);

            // Local command processing
            if (strncmp(buffer, "/help", 5) == 0) {
                print_help();
                continue;
            }
            if (strncmp(buffer, "/quit", 5) == 0) {
                printf("[SYSTEM] Goodbye!\n");
                break;
            }
            // Send message or command to server
            send(sockfd, buffer, strlen(buffer), 0);
        }

        // Server message available
        if (FD_ISSET(sockfd, &read_fds)) {
            int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (n <= 0) {
                printf("[SYSTEM] Server disconnected.\n");
                break;
            }
            buffer[n] = '\0';
            printf("%s", buffer);
        }
    }

    close(sockfd);
    return 0;
}
