#include "util.h"
#include <time.h>

const char *MONTH_NAMES[12] = {"January",   "February", "March",    "April",
                               "May",       "June",     "July",     "August",
                               "September", "October",  "November", "December"};
const int DAYS_NORMAL[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int DAYS_LEAP[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int is_leap(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int get_month_days(int year, int monthz) {
  return is_leap(year) ? DAYS_LEAP[monthz] : DAYS_NORMAL[monthz];
}

void debug_print_date(const char *msg, struct tm &t) {
  log_printf("%s: %04d-%02d-%02d %02d:%02d:%02d", msg, t.tm_year + 1900,
             t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
}
