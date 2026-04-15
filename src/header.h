#pragma once
#include "section.h"
#include "util.h"
#include <cstring>
#include <ctime>
#include <string>

class Header : public Section {
public:
  Header(int y, int x, int h, int w);

  void draw(const CalendarState &state) override;
};
