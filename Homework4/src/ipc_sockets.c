#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/ipc_socket"
#define BUFFER_SIZE 128

void run_server();
void run_client();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [server|client]\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "server") == 0) run_server();
    else if (strcmp(argv[1], "client") == 0) run_client();
    else printf("Invalid mode. Use 'server' or 'client'.\n");
    return 0;
}

void run_server() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    unlink(SOCKET_PATH);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);
    printf("[SERVER] Waiting for client...\n");

    client_fd = accept(server_fd, NULL, NULL);
    printf("[SERVER] Client connected!\n");
    while (1) {
        int n = read(client_fd, buffer, sizeof(buffer)-1);
        if (n <= 0) break;
        buffer[n] = '\0';
        printf("[CLIENT] %s", buffer);
        write(client_fd, buffer, n); // Echo back
    }
    close(client_fd); close(server_fd); unlink(SOCKET_PATH);
}

void run_client() {
    int sockfd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    printf("[CLIENT] Connected! Type to chat:\n");
    while (fgets(buffer, sizeof(buffer), stdin)) {
        write(sockfd, buffer, strlen(buffer));
        int n = read(sockfd, buffer, sizeof(buffer)-1);
        if (n <= 0) break;
        buffer[n] = '\0';
        printf("[ECHO] %s", buffer);
    }
    close(sockfd);
}
