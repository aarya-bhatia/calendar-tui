#pragma once
#include <ncurses.h>
#include "calendar_state.h"

class Section {
public:
    WINDOW* win = NULL;
    int start_y, start_x, height, width;

    Section(int y, int x, int h, int w);

    virtual ~Section();

    virtual void draw(const CalendarState& state) = 0;
    virtual bool handle_input(int ch, CalendarState& state);
};
