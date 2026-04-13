#include "table.h"
#include "util.h"
#include <assert.h>

TableSection::TableSection(int y, int x, int h, int w,
                           const CalendarState &state)
    : Section(y, x, h, w), entries(NUM_ROWS * NUM_COLS) {

  cellh = h / (1+NUM_ROWS);
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

  struct tm last_tm = anchor_tm;
  last_tm.tm_mday += NUM_ENTRIES;
  mktime(&last_tm);

  entries[0].tm_info = anchor_tm;
  for (int i = 1; i < entries.size(); i++) {
    entries[i].tm_info = entries[i - 1].tm_info;
    entries[i].tm_info.tm_mday += 1;
    mktime(&entries[i].tm_info);
  }
}

void TableSection::draw(const CalendarState &state) {
  werase(win);

  for(int c = 0; c < NUM_COLS; c++) {
      mvwprintw(win, 0, cellw * c, "%s", DAY_SHORTNAMES[c]);
  }

  for (int r = 0; r < NUM_ROWS; r++) {
    for (int c = 0; c < NUM_COLS; c++) {
      TableEntry &e = entries[r * NUM_COLS + c];
      if (e.tm_info.tm_year == state.today.tm_year &&
          e.tm_info.tm_mon == state.today.tm_mon &&
          e.tm_info.tm_mday == state.today.tm_mday) {
        mvwprintw(win, cellh * (r+1), cellw * c, "[%d]", e.tm_info.tm_mday);
      } else {
        mvwprintw(win, cellh * (r+1), cellw * c, "%d", e.tm_info.tm_mday);
      }
    }
  }
  wnoutrefresh(win);
}
