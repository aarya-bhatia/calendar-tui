#pragma once

#define log_printf(fmt, ...)                                                   \
  do {                                                                         \
      fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__,   \
              __VA_ARGS__);                                                    \
  } while (0)

extern const char *MONTH_NAMES[];
extern const int DAYS_NORMAL[];
extern const int DAYS_LEAP[];

int is_leap(int year);
int get_month_days(int year, int monthz);
