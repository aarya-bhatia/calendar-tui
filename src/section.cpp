#include "section.h"

Section::Section(int y, int x, int h, int w) : start_y(y), start_x(x), height(h), width(w) {
    win = newwin(h, w, y, x);
    keypad(win, TRUE);
}

Section::~Section() {
    if (win) delwin(win);
}

bool Section::handle_input(int ch, CalendarState& state) {
    return false;
}
