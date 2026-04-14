#pragma once
#include "section.h"
#include "table.h"
#include <ctime>

#define FOOTER_COLOR 3

class FooterSection : public Section {
  TableSection &table;

public:
  FooterSection(TableSection &table, int y, int x, int h, int w)
      : Section(y, x, h, w), table(table) {}

  void draw(const CalendarState &state) override {
    werase(win);
    wbkgd(win, COLOR_PAIR(FOOTER_COLOR));
    wattron(win, COLOR_PAIR(FOOTER_COLOR) | A_BOLD);

    // Fill the whole line with the background color
    mvwhline(win, 0, 0, ' ', width);

    struct tm selected = table.get_selected_date();

    mvwprintw(win, 0, 1, " Date Selected: %04d/%02d/%02d",
              1900 + selected.tm_year, selected.tm_mon + 1,
              selected.tm_mday);

    wattroff(win, COLOR_PAIR(FOOTER_COLOR) | A_BOLD);
    wnoutrefresh(win);
  }
};
