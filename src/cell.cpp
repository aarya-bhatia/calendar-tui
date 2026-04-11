#include "cell.h"
#include "util.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Cell::_init() {
  memset(&tm, 0, sizeof(struct tm));
  win = newwin(param.cell_h, param.cell_w, param.cell_begy, param.cell_begx);
}

void Cell::draw(bool selected = false) {
  if (win == NULL) {
    log_printf("cell draw: win is NULL for mday %d", tm.tm_mday);
    return;
  }
  if(selected) wattron(win, COLOR_PAIR(1));
  box(win, '|', '-');
  mvwprintw(win, 1, 1, "%d", tm.tm_mday);
  if(selected) wattroff(win, COLOR_PAIR(1));
  wnoutrefresh(win);
}
