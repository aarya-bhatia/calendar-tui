#pragma once
#include "util.h"
#include <string.h>
#include <time.h>

struct CalendarState {
  struct tm first_day_of_view_month;
  struct tm today; // TODO: make this dynamic so app works across days

  void refresh_today_date();
  void update_view_date(struct tm day_in_view_month);
  void set_view_prev_month();
  void set_view_next_month();
};
