#include "header.h"

Header::Header(int y, int x, int h, int w) : Section(y, x, h, w) {}

void Header::draw(const CalendarState &state) {
  werase(win);

  wattron(win, COLOR_PAIR(1));
  mvwprintw(win, 0, 0, "[h] help  [arrows/hjkl] navigate  [q] quit");
  wattroff(win, COLOR_PAIR(1));

  const char *month_name = MONTH_NAMES[state.first_day_of_view_month.tm_mon];

  wattron(win, A_BOLD);
  mvwprintw(win, 2, 0, "%s %04d", month_name, 1900 + state.first_day_of_view_month.tm_year);
  wattroff(win, A_BOLD);

  wnoutrefresh(win);
}
