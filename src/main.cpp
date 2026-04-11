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

void recompute_grid();

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
  init_pair(2, 200, COLOR_BLACK);

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
    endwin();
    exit(1);
  }

  debug_print_date("The selected date is ", local_time);
}

void draw_menu() { mvprintw(0, 0, "[h] help  [q] quit"); }

void draw_title() {
  const char *month_selected = MONTH_NAMES[date_selected.tm_mon];
  char title[100];
  std::string text(COLS, ' ');

  snprintf(title, sizeof title, "%s %04d", month_selected,
           1900 + date_selected.tm_year);

  int title_len = strlen(title);
  int start_index = (COLS - title_len) / 2;
  if (start_index < 0)
    start_index = 0;

  for (int i = 0; i < title_len && (start_index + i) < COLS; i++) {
    text[start_index + i] = toupper(title[i]);
  }

  attron(A_BOLD);
  mvprintw(1, 0, "%s", text.c_str());
  attroff(A_BOLD);
}

void draw_footer(int y) {
  mvprintw(y, 0, "Date Selected: %04d/%02d/%02d",
           1900 + date_selected.tm_year, date_selected.tm_mon + 1,
           date_selected.tm_mday);
}

void draw_weekday_names() {
  int cell_width = COLS / 7;
  for (int i = 0; i < 7; i++) {
    const char *name = DAY_SHORTNAMES[i];
    mvprintw(3, i * cell_width, "%s", name);
  }
}

void draw(Grid &grid) {
  erase();
  wnoutrefresh(stdscr);
  draw_menu();
  draw_title();
  draw_weekday_names();
  grid.draw();
  
  int y = grid.param.grid_begy + grid.param.grid_rows * (grid.param.grid_h/grid.param.grid_rows);
  log_printf("footer line: %d", y);
  draw_footer(y);
  wnoutrefresh(stdscr);
  doupdate();
}

void recompute_grid() {
  refresh(); // Updates LINES and COLS
  if (LINES < 10 || COLS / 7 < 5) {
    log_printf("Screen too small: %dx%d", LINES, COLS);
    endwin();
    exit(0);
  }

  if (grid) {
    delete grid;
  }
  GridParam gridinfo;
  gridinfo.grid_h = LINES - 5;
  gridinfo.grid_w = COLS;
  gridinfo.grid_begy = 4;
  gridinfo.grid_begx = 0;
  gridinfo.grid_rows = 5;
  gridinfo.grid_cols = 7;

  grid = new Grid(gridinfo);
  if (grid->set_dates(date_selected.tm_year, date_selected.tm_mon) == -1) {
    log_printf("%s", "Exiting due to error in set_dates()...");
    endwin();
    exit(1);
  }
}

int handle_input() {
  int ch = getch();
  if (ch == 'q') {
    return -1;
  } else if (ch == KEY_RESIZE) {
    recompute_grid();
  } else if (ch == KEY_RIGHT || ch == 'l') {
  } else if (ch == KEY_LEFT || ch == 'h') {
  } else if (ch == KEY_UP || ch == 'k') {
  } else if (ch == KEY_DOWN || ch == 'j') {
  }

  return 0;
}

int main(int argc, char *argv[]) {
  init();

  recompute_grid();

  while (1) {
    if (grid) {
      draw(*grid);
    } else { 
      break;
    }
    if (handle_input() == -1) {
      break;
    }
  }

  if (grid) {
    delete grid;
  }
  endwin();
  return 0;
}
