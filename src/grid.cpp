#include "grid.h"
#include "cell.h"
#include "util.h"

CellParam GridParam::get_cell_param(int cell_index) {
  int cellh = grid_h / grid_rows;
  int cellw = grid_w / grid_cols;
  int celly = cell_index / grid_cols;
  int cellx = cell_index % grid_cols;
  int begy = grid_begy + celly * cellh;
  int begx = grid_begx + cellx * cellw;
  return {cellh, cellw, begy, begx};
}

void Grid::_init() {
  log_printf("%d LINES x %d COLS", LINES, COLS);
  log_printf("Grid size: h:%d w:%d", param.grid_h, param.grid_w);
  log_printf("%d rows x %d cols", param.grid_rows, param.grid_cols);
  log_printf("Grid offset: y:%d x:%d", param.grid_begy, param.grid_begx);

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
  tm_info.tm_hour = 0;
  tm_info.tm_min = 0;
  tm_info.tm_sec = 0;
  if (mktime(&tm_info) == -1) {
    perror("mktime");
    return -1;
  }

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
  char s[100];
  for (int i = 0; i < cells.size(); i++) {
    snprintf(s, sizeof s, "cell %d: ", i);
    debug_print_date(s, cells[i].tm);
  }
}

void Grid::draw() {
  time_t raw_time;
  time(&raw_time);
  struct tm local_time;
  localtime_r(&raw_time, &local_time);

  log_printf("grid draw: drawing %lu cells", (unsigned long)cells.size());
  for(int i = 0; i < (int)cells.size(); i++) {
    bool selected = (cells[i].tm.tm_mday == local_time.tm_mday &&
                     cells[i].tm.tm_mon == local_time.tm_mon &&
                     cells[i].tm.tm_year == local_time.tm_year);
    cells[i].draw(selected);
  }
}
