#pragma once
#include <ncurses.h>
#include "calendar_state.h"

class Section {
public:
    WINDOW* win = nullptr;
    int start_y, height, width;

    Section(int y, int h, int w) : start_y(y), height(h), width(w) {
        win = newwin(h, w, y, 0);
        keypad(win, TRUE);
    }

    virtual ~Section() {
        if (win) delwin(win);
    }

    virtual void draw(const CalendarState& state) = 0;
    virtual bool handle_input(int ch, CalendarState& state) { return false; }
};
