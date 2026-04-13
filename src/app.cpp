#include "app.h"
#include "footer_section.h"
#include "header_section.h"
#include "table.h"
#include "util.h"
#include <assert.h>
#include <fcntl.h>
#include <ncurses.h>
#include <unistd.h>

#define LOG_FILENAME "app.log"
#define MIN_LINES 10
#define MIN_COLS 20

#define SELECTION_COLOR 1
#define OTHER_MONTH_COLOR 2
#define FOOTER_COLOR 3

Application::Application() {
  init_ncurses();
  init_state();
  recompute_layout();
}

Application::~Application() { endwin(); }

void Application::init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  start_color();
  use_default_colors();
  init_pair(Colors::Red, COLOR_RED, -1);
  init_pair(Colors::Blue, COLOR_BLUE, -1);
  init_pair(Colors::Green, COLOR_GREEN, -1);
  init_pair(Colors::Yellow, COLOR_YELLOW, -1);

  if (COLORS >= 256) {
    init_pair(3, COLOR_WHITE, 236);
  } else {
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
  }

  int logfile = open(LOG_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0640);
  dup2(logfile, 2);
  close(logfile);
}

void Application::init_state() {
  time_t raw;
  time(&raw);
  memset(&state.today, 0, sizeof state.today);
  localtime_r(&raw, &state.today);
  state.selected = state.today;
  int res = mktime(&state.selected);
  assert(res != -1);

  state.view_year = state.selected.tm_year;
  state.view_month = state.selected.tm_mon;

  log_printf("today is %d/%d/%d", 1900 + state.selected.tm_year,
             state.selected.tm_mon, state.selected.tm_mday);
}

void Application::recompute_layout() {
  sections.clear();
  erase();
  refresh();

  if (LINES < MIN_LINES || COLS < MIN_COLS)
  {
    quit_signal = true;
    return;
  }

  int header_h = 4;
  int footer_h = 1;
  int table_h = LINES - header_h - footer_h;
  sections.push_back(std::make_unique<HeaderSection>(0, 0, header_h, COLS));
  sections.push_back(
      std::make_unique<TableSection>(header_h, 0, table_h, COLS / 2, state));
  sections.push_back(
      std::make_unique<FooterSection>(header_h + table_h, 0, footer_h, COLS));
}

void Application::run() {
  while (!quit_signal) {
    for (auto &s : sections) {
      s->draw(state);
    }
    doupdate();

    int ch = getch();
    if (ch == 'q') {
      quit_signal = true;
      continue;
    } else if (ch == KEY_RESIZE) {
      recompute_layout();
      continue;
    }

    for (auto &s : sections) {
      if (s->handle_input(ch, state)) {
        break;
      }
    }
  }
}
