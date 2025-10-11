CC = gcc
CFLAGS = -Wall -Wextra -pthread -lrt -g
SRC = src/chat_server.c src/chat_client.c src/ipc_sockets.c src/ipc_mq.c src/message.c src/utils.c
OBJ = $(SRC:.c=.o)
DEPS = include/chat.h include/message.h include/ipc.h

all: chat_server chat_client ipc_sockets ipc_mq

chat_server: src/chat_server.o src/message.o src/utils.o
	$(CC) $(CFLAGS) -o $@ $^ -lrt -pthread

chat_client: src/chat_client.o src/message.o src/utils.o
	$(CC) $(CFLAGS) -o $@ $^ -lrt -pthread

ipc_sockets: src/ipc_sockets.o
	$(CC) $(CFLAGS) -o $@ $^

ipc_mq: src/ipc_mq.o
	$(CC) $(CFLAGS) -o $@ $^ -lrt

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f src/*.o chat_server chat_client ipc_sockets ipc_mq

.PHONY: clean all
