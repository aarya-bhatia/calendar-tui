#include "app.h"
#include "util.h"
#include "view.h"
#include <fcntl.h>
#include <memory.h>
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

Application::Application() {
  setlocale(LC_ALL, "");
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

Application::~Application() { endwin(); }

void Application::recompute_layout() {
  views.clear();
  erase();
  refresh();

  if (LINES < MIN_LINES || COLS < MIN_COLS) {
    log_printf("screen is too small (min: %d x %d)", MIN_LINES, MIN_COLS);
    state.quit();
    return;
  }

  int header_h = 4;
  int footer_h = 1;
  int table_h = 14;
  int events_h = LINES - header_h - table_h - footer_h;

  views.push_back(std::make_unique<HeaderView>(0, 0, header_h, COLS));
  views.push_back(
      std::make_unique<CalendarView>(header_h, 0, table_h, COLS / 2));
  views.push_back(
      std::make_unique<EventView>(header_h + table_h, 0, events_h, COLS));
  views.push_back(std::make_unique<FooterView>(header_h + table_h + events_h, 0,
                                               footer_h, COLS));
}

void Application::run() {
  if (views.empty())
    recompute_layout();
  while (state.is_running()) {
    for (auto &view : views) {
      view->render(state);
    }
    doupdate();
    int ch = getch();
    if (ch == KEY_RESIZE) {
      recompute_layout();
      continue;
    } else if (ch == KEY_F(1)) {
      return; // exit
    }

    if (state.is_typing()) {
      // Handle typing
    } else {
      switch (ch) {
      case 't':
        state.jump_to_date(state.get_today());
        break;
      case KEY_UP:
      case 'k':
        state.move(AppState::TOP);
        break;
      case KEY_DOWN:
      case 'j':
        state.move(AppState::BOTTOM);
        break;
      case KEY_LEFT:
      case 'h':
        state.move(AppState::LEFT);
        break;
      case KEY_RIGHT:
      case 'l':
        state.move(AppState::RIGHT);
        break;
      case 'q':
        state.quit();
        break;
      case 'a':
        state.insert_event("New Event");
        break;
      }
    }
  }
}
