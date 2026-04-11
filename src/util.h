#pragma once
#include <stdio.h>

#define log_printf(fmt, ...)                                                   \
  do {                                                                         \
    fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__);      \
  } while (0)

extern const char *MONTH_NAMES[];
extern const int DAYS_NORMAL[];
extern const int DAYS_LEAP[];

int is_leap(int year);
int get_month_days(int year, int monthz);
void debug_print_date(const char *, struct tm &tm_info);
