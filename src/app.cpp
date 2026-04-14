#include "app.h"
#include "event_section.h"
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

Application::~Application() {
  for (Section *section : sections) {
    delete section;
  }
  sections.clear();
  endwin();
}

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
  state.refresh_today_date();
  state.update_view_date(state.today);
}

void Application::recompute_layout() {
  for (Section *section : sections) {
    delete section;
  }
  sections.clear();
  erase();
  refresh();

  if (LINES < MIN_LINES || COLS < MIN_COLS) {
    quit_signal = true;
    return;
  }

  int header_h = 4;
  int footer_h = 1;
  int table_h = 14;
  int events_h = LINES - header_h - table_h - footer_h;

  sections.push_back(new HeaderSection(0, 0, header_h, COLS));
  Section *table = new TableSection(header_h, 0, table_h, COLS / 2, state);
  sections.push_back(table);
  // sections.push_back(new FooterSection(*((TableSection *)table),
  //                                      header_h + table_h, 0, footer_h,
  //                                      COLS));
  sections.push_back(new EventSection(header_h + table_h, 0, events_h, COLS,
                                      state, *(TableSection *)table));
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
