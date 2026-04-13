#pragma once
#include "section.h"
#include <ctime>

#define FOOTER_COLOR 3

class FooterSection : public Section {
public:
    FooterSection(int y, int x, int h, int w) : Section(y, x, h, w) {}

    void draw(const CalendarState& state) override {
        werase(win);
        wbkgd(win, COLOR_PAIR(FOOTER_COLOR));
        wattron(win, COLOR_PAIR(FOOTER_COLOR) | A_BOLD);
        
        // Fill the whole line with the background color
        mvwhline(win, 0, 0, ' ', width);
        
        mvwprintw(win, 0, 1, " Date Selected: %04d/%02d/%02d",
                 1900 + state.selected.tm_year, state.selected.tm_mon + 1,
                 state.selected.tm_mday);
                 
        wattroff(win, COLOR_PAIR(FOOTER_COLOR) | A_BOLD);
        wnoutrefresh(win);
    }
};
