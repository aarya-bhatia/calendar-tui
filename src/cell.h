#pragma once
#include <ncurses.h>
#include <time.h>
#include "util.h"

#define SELECTION_COLOR 1
#define OTHER_MONTH_COLOR 2

struct Cell {
    WINDOW* win = nullptr;
    struct tm date;

    Cell(WINDOW* parent, int h, int w, int y, int x) {
        win = derwin(parent, h, w, y, x);
    }

    ~Cell() {
        if (win) delwin(win);
    }

    void draw(bool selected, bool today, bool other_month) {
        if (!win) return;
        werase(win);
        
        if (selected) wattron(win, COLOR_PAIR(SELECTION_COLOR));
        if (other_month) wattron(win, COLOR_PAIR(OTHER_MONTH_COLOR));
        
        box(win, '|', '-');
        
        mvwprintw(win, 1, 1, "%2d", date.tm_mday);
        
        if (today) {
            wattron(win, A_BOLD | A_UNDERLINE);
            mvwprintw(win, 1, 4, "(TODAY)");
            wattroff(win, A_BOLD | A_UNDERLINE);
        }

        if (selected) wattroff(win, COLOR_PAIR(SELECTION_COLOR));
        if (other_month) wattroff(win, COLOR_PAIR(OTHER_MONTH_COLOR));
        
        wnoutrefresh(win);
    }
};
