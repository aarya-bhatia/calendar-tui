#pragma once
#include <time.h>

struct CalendarState {
    int view_year;
    int view_month;
    struct tm selected;
    struct tm today;
};
