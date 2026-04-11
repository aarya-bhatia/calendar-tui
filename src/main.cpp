#include "grid.h"
#include "util.h"
#include <assert.h>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define LOG_FILENAME "app.log"
#define MIN_YEAR 1900
#define MAX_YEAR 9999

void init() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  refresh();

  int logfile = open(LOG_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0640);
  dup2(logfile, 2);
  close(logfile);
}

void draw() {
  clear();

  doupdate();
}

void handle_input() {
  int ch = getch();
  if (ch == 'q') {
  } else if (ch == KEY_RIGHT || ch == 'l') {
  } else if (ch == KEY_LEFT || ch == 'h') {
  } else if (ch == KEY_UP || ch == 'k') {
  } else if (ch == KEY_DOWN || ch == 'j') {
  }
}

int main(int argc, char *argv[]) {
  GridParam gridinfo;
  gridinfo.grid_h = LINES - 3;
  gridinfo.grid_w = COLS;
  gridinfo.grid_begy = 0;
  gridinfo.grid_begx = 0;
  gridinfo.grid_rows = 5;
  gridinfo.grid_cols = 7;

  Grid grid(gridinfo);

  init();

  time_t raw_time;
  time(&raw_time);
  struct tm local_time;
  localtime_r(&raw_time, &local_time);
  grid.set_dates(local_time.tm_year, local_time.tm_mon);
  grid.debug_print();

  // while (1) {
  //   draw();
  //   handle_input();
  // }

  endwin();
}
