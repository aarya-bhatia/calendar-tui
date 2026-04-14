#pragma once
#include "calendar_state.h"
#include "section.h"
#include "util.h"
#include <ncurses.h>

class TableSection : public Section {
  struct tm anchor_tm;
  int selected_entry_index = 0;
  int cellh, cellw;

  int get_today_entry_index(const struct tm &today);

public:
  static const int NUM_ROWS = 6;
  static const int NUM_COLS = 7;
  static const int NUM_ENTRIES = NUM_ROWS * NUM_COLS;

  static const unsigned int attr_selected_entry =
      COLOR_PAIR(Colors::Yellow) | A_BOLD;
  static const unsigned int attr_today_entry =
      COLOR_PAIR(Colors::Red) | A_UNDERLINE;
  static const unsigned int attr_outside_month_entry = COLOR_PAIR(Colors::Blue);

  TableSection(int y, int x, int h, int w, const CalendarState &state);

  void draw(const CalendarState &state) override;
  bool handle_input(int ch, CalendarState &state) override;
};
