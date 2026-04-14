#pragma once
#include "util.h"
#include <string.h>
#include <time.h>

struct CalendarState {
  struct tm first_day_of_view_month;
  struct tm today; // TODO: make this dynamic so app works across days

  void refresh_today_date() {
    time_t raw;
    time(&raw);
    memset(&today, 0, sizeof today);
    localtime_r(&raw, &today);
    debug_print_date("Today is ", today);
  }

  void update_view_date(struct tm day_in_view_month) {
    struct tm t;
    memset(&t, 0, sizeof t);
    t.tm_mday = 1;
    t.tm_year = day_in_view_month.tm_year;
    t.tm_mon = day_in_view_month.tm_mon;
    mktime(&t);
    first_day_of_view_month = t;
  }

  void set_view_prev_month() {
    first_day_of_view_month.tm_mon--;
    mktime(&first_day_of_view_month);
  }

  void set_view_next_month() {
    first_day_of_view_month.tm_mon++;
    mktime(&first_day_of_view_month);
  }
};
