#include "util.h"
#include "view.h"
#include "app_state.h"
#include <assert.h>
#include <ncurses.h>

CalendarView::CalendarView(int y, int x, int h, int w) : View(y, x, h, w) {
  cellh = h / (1 + AppState::CALENDAR_NUM_ROWS);
  cellw = w / AppState::CALENDAR_NUM_COLS;

  assert(cellh >= 1);
  assert(cellw >= 1);
}

void CalendarView::render(const AppState &state) {
  werase(win);

  for (int c = 0; c < AppState::CALENDAR_NUM_COLS; c++) {
    mvwprintw(win, 0, cellw * c, "%s", DAY_SHORTNAMES[c]);
  }

  for (int r = 0; r < AppState::CALENDAR_NUM_ROWS; r++) {
    for (int c = 0; c < AppState::CALENDAR_NUM_COLS; c++) {
      int entry_index = r * AppState::CALENDAR_NUM_COLS + c;
      struct tm entry_date = state.get_entry_date(entry_index);
      int entry_y = cellh * (r + 1);
      int entry_x = cellw * c;

      bool has_events = !state.get_events(entry_date).empty();

      if (entry_index == state.get_selected_index() &&
          date_equals(entry_date, state.get_today())) {
        wattron(win, attr_selected_entry | A_UNDERLINE);
        mvwprintw(win, entry_y, entry_x, "[%d]", entry_date.tm_mday);
        wattroff(win, attr_selected_entry | A_UNDERLINE);
      } else if (entry_index == state.get_selected_index()) {
        wattron(win, attr_selected_entry);
        mvwprintw(win, entry_y, entry_x, "[%d]", entry_date.tm_mday);
        wattroff(win, attr_selected_entry);
      } else if (date_equals(entry_date, state.get_today())) {
        wattron(win, attr_today_entry);
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
        wattroff(win, attr_today_entry);
      } else if (entry_date.tm_mon != state.get_view_month().tm_mon) {
        wattron(win, attr_outside_month_entry);
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
        wattroff(win, attr_outside_month_entry);
      } else {
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
      }

      if(has_events) wprintw(win, "\u002a");
    }
  }
  wnoutrefresh(win);
}

void FooterView::render(const AppState &state) {
  werase(win);
  wbkgd(win, COLOR_PAIR(FOOTER_COLOR));
  wattron(win, COLOR_PAIR(FOOTER_COLOR) | A_BOLD);

  // Fill the whole line with the background color
  mvwhline(win, 0, 0, ' ', width);

  struct tm selected = state.get_selected_date();

  mvwprintw(win, 0, 1, " Date Selected: %04d/%02d/%02d",
            1900 + selected.tm_year, selected.tm_mon + 1,
            selected.tm_mday);

  wattroff(win, COLOR_PAIR(FOOTER_COLOR) | A_BOLD);
  wnoutrefresh(win);
}

void HeaderView::render(const AppState &state) {
  werase(win);

  wattron(win, COLOR_PAIR(1));
  mvwprintw(win, 0, 0, "[h] help [hjklHJKL] navigate [q] quit [t] go to today");
  wattroff(win, COLOR_PAIR(1));

  const char *month_name = MONTH_NAMES[state.get_view_month().tm_mon];

  wattron(win, A_BOLD);
  mvwprintw(win, 2, 0, "%s %04d", month_name, 1900 + state.get_view_month().tm_year);
  wattroff(win, A_BOLD);

  wnoutrefresh(win);
}


void EventView::render(const AppState &state) {
  werase(win);
  wmove(win, 0, 0);
  if(state.is_typing()) {
    if(state.event_edit_mode == AppState::ADD_EVENT) {
      static const char *prompt = "[INSERT] Enter event description (or leave blank) > ";
      wprintw(win, prompt);
      wprintw(win, state.typing_buffer.c_str());
      move(start_y, start_x + strlen(prompt) + state.typing_buffer.size());
    } else {
      static const char *prompt = "[DELETE] Enter event ID (or leave blank) > ";
      wprintw(win, prompt);
      wprintw(win, state.typing_buffer.c_str());
      move(start_y, start_x + strlen(prompt) + state.typing_buffer.size());
    }
  } else {
    wprintw(win, "Press a to add event, d to delete event.");
  }

  struct tm t = state.get_selected_date();
  const std::vector<AppState::Event> &v = state.get_events(t);
  for (int i = 0; i < (int)v.size(); i++) {
    log_printf("Drawing event %d...", v[i].id);
    mvwprintw(win, i+2, 0, "(%d) %s", v[i].id, v[i].description.c_str());
  }
  wnoutrefresh(win);
}
