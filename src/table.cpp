#include "table.h"
#include "util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TableSection::TableSection(int y, int x, int h, int w,
                           const CalendarState &state)
    : Section(y, x, h, w) {

  cellh = h / (1 + NUM_ROWS);
  cellw = w / NUM_COLS;

  cellh = 2;
  cellw = 6;

  assert(cellh >= 1);
  assert(cellw >= 1);

  anchor_tm = get_anchor_date(state.first_day_of_view_month.tm_year,
                              state.first_day_of_view_month.tm_mon);
  debug_print_date("anchor date: ", anchor_tm);
  selected_entry_index = get_today_entry_index(state.today);
  log_printf("initial selected entry: %d", selected_entry_index);
}

void TableSection::update_selection(struct tm new_selection_date,
                                    CalendarState &state) {
  struct tm new_anchor =
      get_anchor_date(new_selection_date.tm_year, new_selection_date.tm_mon);
  // check if anchor has changed.
  if (new_anchor.tm_year != anchor_tm.tm_year ||
      new_anchor.tm_mon != anchor_tm.tm_mon) {
    anchor_tm = new_anchor;
    state.update_view_date(new_selection_date);
  }

  this->selected_entry_index = get_entry_index(new_selection_date);
}

int TableSection::get_entry_index(struct tm date) {
  time_t t1 = mktime(&anchor_tm);
  time_t t2 = mktime(&date);
  return abs(t2 - t1) / SECONDS_IN_DAY;
}

struct tm TableSection::get_entry_date(int index) {
  struct tm entry_date = anchor_tm;
  entry_date.tm_mday += index;
  mktime(&entry_date);
  return entry_date;
}

struct tm TableSection::get_anchor_date(int view_year, int view_month) {
  struct tm t;
  memset(&t, 0, sizeof t);

  t.tm_year = view_year;
  t.tm_mon = view_month;
  t.tm_mday = 1;
  t.tm_isdst = -1;
  mktime(&t);

  t.tm_mday -= t.tm_wday;
  mktime(&t);

  return t;
}

int TableSection::get_today_entry_index(struct tm today) {
  time_t t1 = mktime(&anchor_tm);
  time_t t2 = mktime(&today);
  return abs(t2 - t1) / SECONDS_IN_DAY;
}

void TableSection::draw(const CalendarState &state) {
  werase(win);

  for (int c = 0; c < NUM_COLS; c++) {
    mvwprintw(win, 0, cellw * c, "%s", DAY_SHORTNAMES[c]);
  }

  for (int r = 0; r < NUM_ROWS; r++) {
    for (int c = 0; c < NUM_COLS; c++) {
      int entry_index = r * NUM_COLS + c;
      struct tm entry_date = get_entry_date(entry_index);
      int entry_y = cellh * (r + 1);
      int entry_x = cellw * c;

      if (entry_index == selected_entry_index &&
          date_equals(entry_date, state.today)) {
        wattron(win, attr_selected_entry | A_UNDERLINE);
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
        wattroff(win, attr_selected_entry | A_UNDERLINE);
      } else if (entry_index == selected_entry_index) {
        wattron(win, attr_selected_entry);
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
        wattroff(win, attr_selected_entry);
      } else if (date_equals(entry_date, state.today)) {
        wattron(win, attr_today_entry);
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
        wattroff(win, attr_today_entry);
      } else if (entry_date.tm_mon != state.first_day_of_view_month.tm_mon) {
        wattron(win, attr_outside_month_entry);
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
        wattroff(win, attr_outside_month_entry);
      } else {
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
      }
    }
  }
  wnoutrefresh(win);
}

struct tm TableSection::get_selected_date() const {
  struct tm t = anchor_tm;
  t.tm_mday += selected_entry_index;
  mktime(&t);
  debug_print_date("TableSection:: Selected date: ", t);
  log_printf("entry index: %d", selected_entry_index);
  return t;
}

bool TableSection::handle_input(int ch, CalendarState &state) {
  // jump to today
  if (ch == 't') {
    update_selection(state.today, state);
    return true;
  }

  // navigation
  if (ch == KEY_RIGHT || ch == 'l') {
    selected_entry_index++;
  } else if (ch == KEY_LEFT || ch == 'h') {
    selected_entry_index--;
  } else if (ch == KEY_UP || ch == 'k') {
    selected_entry_index -= 7;
  } else if (ch == KEY_DOWN || ch == 'j') {
    selected_entry_index += 7;
  }

  if (selected_entry_index < 0) {
    selected_entry_index += NUM_ENTRIES;
    anchor_tm.tm_mday -= NUM_ENTRIES;
    mktime(&anchor_tm);
    state.set_view_prev_month();
  } else if (selected_entry_index >= NUM_ENTRIES) {
    selected_entry_index -= NUM_ENTRIES;
    anchor_tm.tm_mday += NUM_ENTRIES;
    mktime(&anchor_tm);
    state.set_view_next_month();
  }
  return true;
}
