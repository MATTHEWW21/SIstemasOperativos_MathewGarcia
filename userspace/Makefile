CC = gcc
CFLAGS = -Wall -Wextra -O2

all: test_msgbuf test_concurrent test_stress

test_msgbuf: test_msgbuf.c
	$(CC) $(CFLAGS) -o test_msgbuf test_msgbuf.c

test_concurrent: test_concurrent.c
	$(CC) $(CFLAGS) -o test_concurrent test_concurrent.c

test_stress: test_stress.c
	$(CC) $(CFLAGS) -o test_stress test_stress.c

clean:
	rm -f test_msgbuf test_concurrent test_stress
