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

  memset(&anchor_tm, 0, sizeof anchor_tm);
  anchor_tm.tm_year = state.view_year;
  anchor_tm.tm_mon = state.view_month;
  anchor_tm.tm_mday = 1;
  anchor_tm.tm_isdst = -1;
  mktime(&anchor_tm);

  anchor_tm.tm_mday -= anchor_tm.tm_wday;
  mktime(&anchor_tm);

  selected_entry_index = get_today_entry_index(state.today);
}

int TableSection::get_today_entry_index(const struct tm &today) {
  for (int r = 0; r < NUM_ROWS; r++) {
    for (int c = 0; c < NUM_COLS; c++) {
      struct tm entry_date = anchor_tm;
      entry_date.tm_mday += (r * NUM_COLS + c);
      mktime(&entry_date);
      if (date_equals(entry_date, today)) {
        return r * NUM_COLS + c;
      }
    }
  }

  return -1;
}

void TableSection::draw(const CalendarState &state) {
  werase(win);

  for (int c = 0; c < NUM_COLS; c++) {
    mvwprintw(win, 0, cellw * c, "%s", DAY_SHORTNAMES[c]);
  }

  for (int r = 0; r < NUM_ROWS; r++) {
    for (int c = 0; c < NUM_COLS; c++) {
      struct tm entry_date = anchor_tm;
      entry_date.tm_mday += (r * NUM_COLS + c);
      mktime(&entry_date);
      int entry_y = cellh * (r + 1);
      int entry_x = cellw * c;

      if (date_equals(entry_date, state.selected) && date_equals(entry_date, state.today)) {
        wattron(win, attr_selected_entry | A_UNDERLINE);
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
        wattroff(win, attr_selected_entry | A_UNDERLINE);
      } else if (date_equals(entry_date, state.selected)) {
        wattron(win, attr_selected_entry);
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
        wattroff(win, attr_selected_entry);
      } else if (date_equals(entry_date, state.today)) {
        wattron(win, attr_today_entry);
        mvwprintw(win, entry_y, entry_x, "%d", entry_date.tm_mday);
        wattroff(win, attr_today_entry);
      } else if (entry_date.tm_mon != state.view_month) {
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

bool TableSection::handle_input(int ch, CalendarState &state) {
  if (ch == KEY_RIGHT || ch == 'l') {
    selected_entry_index++;
    state.selected.tm_mday += 1;
  } else if (ch == KEY_LEFT || ch == 'h') {
    selected_entry_index--;
    state.selected.tm_mday -= 1;
  } else if (ch == KEY_UP || ch == 'k') {
    selected_entry_index -= 7;
    state.selected.tm_mday -= 7;
  } else if (ch == KEY_DOWN || ch == 'j') {
    selected_entry_index += 7;
    state.selected.tm_mday += 7;
  }
  mktime(&state.selected);
  if (selected_entry_index < 0) {
    selected_entry_index += 42;
    anchor_tm.tm_mday -= 42;
    mktime(&anchor_tm);
    state.view_month--;
    if (state.view_month < 0) {
      state.view_year--;
      state.view_month += 12;
    }
  } else if (selected_entry_index >= 42) {
    selected_entry_index -= 42;
    anchor_tm.tm_mday += 42;
    mktime(&anchor_tm);
    state.view_month++;
    if (state.view_month >= 12) {
      state.view_year++;
      state.view_month -= 12; }
  }
  return true;
}
