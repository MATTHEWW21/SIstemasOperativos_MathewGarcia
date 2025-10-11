#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>

#define QUEUE_NAME "/myqueue"
#define BUFFER_SIZE 128

void run_send();
void run_recv();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [send|recv]\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "send") == 0) run_send();
    else if (strcmp(argv[1], "recv") == 0) run_recv();
    else printf("Invalid mode. Use 'send' or 'recv'.\n");
    return 0;
}

void run_send() {
    mqd_t mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, NULL);
    char buffer[BUFFER_SIZE];
    printf("Type message:\n");
    while (fgets(buffer, sizeof(buffer), stdin)) {
        mq_send(mq, buffer, strlen(buffer)+1, 0);
    }
    mq_close(mq); mq_unlink(QUEUE_NAME);
}

void run_recv() {
    mqd_t mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, NULL);
    char buffer[BUFFER_SIZE];
    while (1) {
        ssize_t n = mq_receive(mq, buffer, BUFFER_SIZE, NULL);
        if (n <= 0) break;
        buffer[n] = '\0';
        printf("Received: %s", buffer);
    }
    mq_close(mq); mq_unlink(QUEUE_NAME);
}
