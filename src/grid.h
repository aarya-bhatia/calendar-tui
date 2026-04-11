#pragma once
#include "util.h"
#include <ncurses.h>
#include <time.h>
#include <vector>

struct CellParam {
  int cell_h, cell_w, cell_begy, cell_begx;
};

struct GridParam {
  int grid_h, grid_w, grid_rows, grid_cols, grid_begy, grid_begx;
  CellParam get_cell_param(int cell_index);
};

struct Cell {
  CellParam param;
  struct tm tm;
  WINDOW *win = NULL;

  Cell(CellParam param) : param(param) { _init(); }
  void _init();
  void draw();
};

class Grid {
  GridParam param;
  std::vector<Cell> cells;

public:
  Grid(GridParam param) : param(param) { _init(); }
  void _init();
  int set_dates(int year, int month);
  void draw();
  void debug_print();
};
