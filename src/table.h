#pragma once
#include "calendar_state.h"
#include "section.h"
#include <ncurses.h>
#include <vector>

struct TableEntry {
  struct tm tm_info;
};

class TableSection : public Section {
  struct tm anchor_tm;
  std::vector<TableEntry> entries;
  int selected = 0;
  int cellh, cellw;

public:
  static const int NUM_ROWS = 6;
  static const int NUM_COLS = 7;
  static const int NUM_ENTRIES = NUM_ROWS * NUM_COLS;

  TableSection(int y, int x, int h, int w, const CalendarState &state);

  void draw(const CalendarState &state) override;
  // bool handle_input(int ch, CalendarState &state) override;
};
