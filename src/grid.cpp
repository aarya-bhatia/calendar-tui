#include "grid.h"
#include "util.h"

CellParam GridParam::get_cell_param(int cell_index) {
  int cellh = grid_h / grid_rows;
  int cellw = grid_w / grid_cols;
  int celly = cell_index / grid_rows;
  int cellx = cell_index % grid_cols;
  int begy = grid_begy + celly * cellh;
  int begx = grid_begx + cellx * cellw;
  return {cellh, cellw, begy, begx};
}

void Cell::_init() {
  memset(&tm, 0, sizeof(struct tm));
  win = newwin(param.cell_h, param.cell_w, param.cell_begy, param.cell_begx);
}

void Grid::_init() {
  cells.reserve(param.grid_rows * param.grid_cols);
  for (int i = 0; i < param.grid_rows * param.grid_cols; i++) {
    cells.emplace_back(param.get_cell_param(i));
  }
}

int Grid::set_dates(int year, int month) {
  struct tm tm_info; // first day of month
  memset(&tm_info, 0, sizeof tm_info);
  tm_info.tm_year = year;
  tm_info.tm_mon = month;
  tm_info.tm_mday = 1;
  if (mktime(&tm_info) == -1) {
    perror("mktime");
    return -1;
  }
  int days_in_month = get_month_days(year, month);
  int first_weekday = tm_info.tm_wday;

  struct tm tm_begin;
  memcpy(&tm_begin, &tm_info, sizeof tm_info);
  tm_begin.tm_mday -= tm_info.tm_wday;
  if (mktime(&tm_begin) == -1) {
    perror("mktime");
    return -1;
  }
  for (int i = 0; i < cells.size(); i++) {
    cells[i].tm = tm_begin;
    tm_begin.tm_mday += 1;
    if (mktime(&tm_begin) == -1) {
      perror("mktime");
      return -1;
    }
  }

  return 0;
}

void Grid::debug_print() {
  for (int i = 0; i < cells.size(); i++) {
    log_printf("Cell %d: %d/%d/%d", i, cells[i].tm.tm_year, cells[i].tm.tm_mon,
               cells[i].tm.tm_mday);
  }
}
