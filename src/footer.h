#pragma once
#include "section.h"
#include "calendar.h"
#include <ctime>

#define FOOTER_COLOR 3

class Footer : public Section {
  Calendar &table;

public:
  Footer(Calendar &table, int y, int x, int h, int w);

  void draw(const CalendarState &state) override;
};
