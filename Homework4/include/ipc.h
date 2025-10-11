#ifndef IPC_H
#define IPC_H

#include <semaphore.h>

void sem_wait_safe(sem_t *sem);
void fatal_error(const char *msg);

#endif
