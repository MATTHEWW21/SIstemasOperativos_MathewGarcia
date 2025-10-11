#ifndef CHAT_H
#define CHAT_H

#include "message.h"

#define MAX_CLIENTS 10

typedef struct {
    int fd;
    char name[USERNAME_MAX];
    int active;
} client_t;

// Helper functions for client management:
int find_client_by_name(client_t *clients, int max_clients, const char *name);
int count_active_clients(client_t *clients, int max_clients);
void trim_newline(char *str);

#endif
