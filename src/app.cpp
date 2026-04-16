#include "app.h"
#include "util.h"
#include "view.h"
#include <fcntl.h>
#include <memory.h>
#include <ncurses.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define CTRL(c) ((c) & 0x1F)

Application::Application() {
  int logfile = open(LOG_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0640);
  dup2(logfile, 2);
  close(logfile);

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

  if (mkdir(DATA_DIRECTORY, 0777) < 0) {
    perror("mkdir");
  } else {
    log_printf("created data directory: %s", DATA_DIRECTORY);
  }
  state.load_events();
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
  views.push_back(std::make_unique<CalendarView>(header_h, 0, table_h, COLS));

  int events_y = header_h + table_h;
  views.push_back(std::make_unique<EventView>(events_y, 0, events_h, COLS));

  int footer_y = header_h + table_h + events_h;
  views.push_back(std::make_unique<FooterView>(footer_y, 0, footer_h, COLS));
}

void Application::render() {
  for (auto &view : views) {
    view->render(state);
  }
  doupdate();
}

void Application::handle_input_typing(int ch) {
  log_printf("Key: %d (typing)", ch);
  if (ch == KEY_ENTER || ch == '\n' || ch == '\r') {
    curs_set(0);
    state.set_typing_off();
  } else if(ch == KEY_BACKSPACE) {
    state.typing_buffer.pop_back();
  } else if(ch == CTRL('u')) {
    state.typing_buffer = "";
    state.typing_buffer.pop_back();
  } else if (isprint(ch)) {
    state.typing_buffer += ch;
  }
}

void Application::handle_input_actions(int ch) {
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
    curs_set(1);
    state.set_typing_on();
    state.typing_buffer = "";
    state.event_edit_mode = AppState::ADD_EVENT;
    break;
  case 'd':
    curs_set(1);
    state.set_typing_on();
    state.typing_buffer = "";
    state.event_edit_mode = AppState::DELETE_EVENT;
    break;
  case 'H':
    state.selected_entry_index -=
        (state.selected_entry_index % AppState::CALENDAR_NUM_COLS);
    break;
  case 'L':
    state.selected_entry_index +=
        AppState::CALENDAR_NUM_COLS - 1 -
        (state.selected_entry_index % AppState::CALENDAR_NUM_COLS);
    break;
  case 'J':
    state.view_next_month_with_same_selection();
    break;
  case 'K':
    state.view_prev_month_with_same_selection();
    break;
  }
}

void Application::handle_input() {
  int ch = getch();
  if (ch == KEY_RESIZE) {
    recompute_layout();
    return;
  } else if (ch == KEY_F(1)) {
    return; // exit
  }

  if (state.is_typing()) {
    handle_input_typing(ch);
  } else {
    handle_input_actions(ch);
  }
}

void Application::run() {
  if (views.empty())
    recompute_layout();
  while (state.is_running()) {
    render();
    handle_input();
  }
}
