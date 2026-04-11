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

Grid *grid = NULL;
struct tm local_time;
struct tm date_selected;

void init() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  curs_set(0);
  refresh();

  start_color();
  use_default_colors();
  init_pair(1, COLOR_RED, -1);

  refresh();

  int logfile = open(LOG_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0640);
  dup2(logfile, 2);
  close(logfile);

  time_t raw_time;
  time(&raw_time);
  localtime_r(&raw_time, &local_time);
  debug_print_date("The date today is ", local_time);

  memset(&date_selected, 0, sizeof date_selected);
  date_selected.tm_year = local_time.tm_year;
  date_selected.tm_mon = local_time.tm_mon;
  date_selected.tm_mday = local_time.tm_mday;
  if (mktime(&date_selected) == -1) {
    perror("mktime");
    exit(1);
  }

  debug_print_date("The selected date is ", local_time);
}

void draw_menu() { mvprintw(0, 0, "[h] help  [q] quit"); }

void draw_title() {
  const char *month_selected = MONTH_NAMES[date_selected.tm_mon];
  char title[100];
  char text[100];
  memset(title, 0, sizeof title);
  memset(text, 0, sizeof title);
  snprintf(title, sizeof title, "%s %04d", month_selected,
           1900 + date_selected.tm_year);
  attron(A_BOLD);
  int title_len = strlen(title);
  int start_index = (COLS - title_len) / 2;
  for (int i = 0; i < COLS; i++) {
    if (i < start_index) {
      text[i] = ' ';
    } else if (i >= start_index && i < start_index + title_len) {
      text[i] = toupper(title[i - start_index]);
    }
  }
  mvprintw(1, 0, text);
  attroff(A_BOLD);
}

void draw_footer() {
  mvprintw(LINES - 1, 0, "Date Selected: %04d/%02d/%02d",
           1900 + date_selected.tm_year, date_selected.tm_mon,
           date_selected.tm_mday);
}

void draw(Grid &grid) {
  erase();
  wnoutrefresh(stdscr);
  draw_menu();
  draw_title();
  grid.draw();
  draw_footer();
  wnoutrefresh(stdscr);
  doupdate();
}

int handle_input() {
  int ch = getch();
  if (ch == 'q') {
    return -1;
  } else if (ch == KEY_RIGHT || ch == 'l') {
  } else if (ch == KEY_LEFT || ch == 'h') {
  } else if (ch == KEY_UP || ch == 'k') {
  } else if (ch == KEY_DOWN || ch == 'j') {
  }

  return 0;
}

void update(Grid &grid) {
  grid.set_dates(local_time.tm_year, local_time.tm_mon);
  grid.debug_print();
}

int main(int argc, char *argv[]) {
  init();

  GridParam gridinfo;
  gridinfo.grid_h = LINES - 3;
  gridinfo.grid_w = COLS;
  gridinfo.grid_begy = 2;
  gridinfo.grid_begx = 0;
  gridinfo.grid_rows = 5;
  gridinfo.grid_cols = 7;

  grid = new Grid(gridinfo);
  update(*grid);

  while (1) {
    draw(*grid);
    if (handle_input() == -1) {
      break;
    }
  }

  delete grid;
  endwin();
}
