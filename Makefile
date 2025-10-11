CC = gcc
CFLAGS = -g -Wall -Iinclude
GTKFLAGS = `pkg-config --cflags --libs gtk+-3.0`
NCURSES_FLAG = -lncurses

SRC = src/scheduler.c src/algorithms.c src/metrics.c src/report.c
GTK_SRC = $(SRC) src/gui_gtk.c
NCURSES_SRC = $(SRC) src/gui_ncurses.c

BIN_GTK = scheduler_gtk
BIN_NCURSES = scheduler_ncurses

TESTS = tests/test_fifo.c tests/test_sjf.c tests/test_stcf.c tests/test_rr.c tests/test_mlfq.c

all: $(BIN_GTK) $(BIN_NCURSES)

$(BIN_GTK): $(GTK_SRC)
	$(CC) $(CFLAGS) -o $@ $(GTK_SRC) $(GTKFLAGS)

$(BIN_NCURSES): $(NCURSES_SRC)
	$(CC) $(CFLAGS) -o $@ $(NCURSES_SRC) $(NCURSES_FLAG)

# Individual algorithm test builds
test_%: tests/test_%.c src/algorithms.c src/metrics.c src/scheduler.c
	$(CC) $(CFLAGS) -o $@ $^
	
tests: test_fifo test_sjf test_stcf test_rr test_mlfq

clean:
	rm -f $(BIN_GTK) $(BIN_NCURSES) test_fifo test_sjf test_stcf test_rr test_mlfq

.PHONY: all clean tests
