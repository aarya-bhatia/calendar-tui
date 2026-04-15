#pragma once
#include <stdio.h>
#include <string>

#define log_puts(s)                                                            \
  do {                                                                         \
    fprintf(stderr, "%s:%d: %s\n", s, __FILE__, __LINE__, s);                  \
  } while (0)

#define log_printf(fmt, ...)                                                   \
  do {                                                                         \
    fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__);      \
  } while (0)

extern const char *DAY_NAMES[];
extern const char *DAY_SHORTNAMES[];
extern const char *MONTH_NAMES[];
extern const int DAYS_NORMAL[];
extern const int DAYS_LEAP[];

const int SECONDS_IN_DAY = 86400;

int is_leap(int year);
int get_month_days(int year, int monthz);
void debug_print_date(const char *, struct tm &tm_info);

enum Colors { Red = 1, Blue, Green, Yellow };

bool date_equals(struct tm t1, struct tm t2);

extern const char *NOTIFICATION_ENDPOINT_URL;

bool push_notification(const std::string &message);
