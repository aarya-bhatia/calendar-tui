#pragma once
#include <ncurses.h>
#include <time.h>

struct CellParam {
  int cell_h, cell_w, cell_begy, cell_begx;
};

struct Cell {
  CellParam param;
  struct tm tm;
  WINDOW *win = NULL;

  Cell(CellParam param) : param(param) { _init(); }
  void _init();
  void draw(bool selected);
};
