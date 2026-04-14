#pragma once
#include "section.h"
#include "util.h"
#include <cstring>
#include <ctime>
#include <string>

class HeaderSection : public Section {
public:
  HeaderSection(int y, int x, int h, int w) : Section(y, x, h, w) {}

  void draw(const CalendarState &state) override {
    werase(win);

    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, 0, 0, "[h] help  [arrows/hjkl] navigate  [q] quit");
    wattroff(win, COLOR_PAIR(1));

    const char *month_name = MONTH_NAMES[state.view_month];

    wattron(win, A_BOLD);
    mvwprintw(win, 2, 0, "%s %04d", month_name, 1900 + state.view_year);
    wattroff(win, A_BOLD);

    wnoutrefresh(win);
  }
};
