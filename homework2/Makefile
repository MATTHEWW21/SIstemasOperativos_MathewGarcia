CC = gcc
CFLAGS = -Wall -pthread
SRC = src
BIN = bin

all: $(SRC)/zombie_creator $(SRC)/zombie_detector $(SRC)/zombie_reaper $(SRC)/process_daemon libzombie.a

$(SRC)/zombie_creator: $(SRC)/zombie_creator.c
	$(CC) $(CFLAGS) -o $(SRC)/zombie_creator $(SRC)/zombie_creator.c

$(SRC)/zombie_detector: $(SRC)/zombie_detector.c
	$(CC) $(CFLAGS) -o $(SRC)/zombie_detector $(SRC)/zombie_detector.c

$(SRC)/zombie_reaper: $(SRC)/zombie_reaper.c
	$(CC) $(CFLAGS) -o $(SRC)/zombie_reaper $(SRC)/zombie_reaper.c

$(SRC)/process_daemon: $(SRC)/process_daemon.c
	$(CC) $(CFLAGS) -o $(SRC)/process_daemon $(SRC)/process_daemon.c

libzombie.a: $(SRC)/zombie.c $(SRC)/zombie.h
	$(CC) $(CFLAGS) -c $(SRC)/zombie.c -o $(SRC)/zombie.o
	ar rcs libzombie.a $(SRC)/zombie.o

clean:
	rm -f $(SRC)/zombie_creator $(SRC)/zombie_detector $(SRC)/zombie_reaper $(SRC)/process_daemon $(SRC)/*.o libzombie.a

.PHONY: all clean
