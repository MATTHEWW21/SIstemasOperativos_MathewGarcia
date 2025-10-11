#include <ncurses.h>
void run_ncurses_ui() {
    initscr();
    cbreak();
    noecho();
    printw("CPU Scheduler Simulator - ncurses UI\n");
    printw("Press any key to exit...");
    refresh();
    getch();
    endwin();
}
