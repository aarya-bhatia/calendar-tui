#pragma once
#include "util.h"
#include "cell.h"
#include <ncurses.h>
#include <time.h>
#include <vector>

struct GridParam {
  int grid_h, grid_w, grid_rows, grid_cols, grid_begy, grid_begx;
  CellParam get_cell_param(int cell_index);
};

class Grid {
  std::vector<Cell> cells;

public:
  GridParam param;
  Grid(GridParam param) : param(param) { _init(); }
  void _init();
  int set_dates(int year, int month);
  void draw();
  void debug_print();
};
