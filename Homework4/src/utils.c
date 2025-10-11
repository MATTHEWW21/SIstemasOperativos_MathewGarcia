#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h" // If your message protocol is in message.h

// Prints a critical error and exits the program
void fatal_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Formats a timestamp as human-readable string
void format_time(time_t timestamp, char *buffer, size_t bufsize) {
    struct tm *tm_info = localtime(&timestamp);
    strftime(buffer, bufsize, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Removes a trailing newline ('\n') from any string
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

// Gets the client index by username, -1 if not found
int find_client_by_name(client_t *clients, int max_clients, const char *name) {
    for (int i = 0; i < max_clients; i++) {
        if (clients[i].active && strcmp(clients[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Counts the number of connected clients
int count_active_clients(client_t *clients, int max_clients) {
    int count = 0;
    for (int i = 0; i < max_clients; i++)
        if (clients[i].active) count++;
    return count;
}
