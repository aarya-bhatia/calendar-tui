#pragma once
#include "section.h"
#include "util.h"
#include <string>
#include <cstring>
#include <ctime>

class HeaderSection : public Section {
public:
    HeaderSection(int y, int h, int w) : Section(y, h, w) {}

    void draw(const CalendarState& state) override {
        werase(win);
        // Menu
        mvwprintw(win, 0, 0, "[h] help  [arrows/hjkl] navigate  [q] quit");

        // Title (Centered)
        const char *month_name = MONTH_NAMES[state.selected.tm_mon];
        char title[100];
        snprintf(title, sizeof title, "%s %04d", month_name, 1900 + state.selected.tm_year);
        
        int title_len = strlen(title);
        int start_index = (width - title_len) / 2;
        if (start_index < 0) start_index = 0;

        wattron(win, A_BOLD);
        mvwprintw(win, 1, start_index, "%s", title);
        wattroff(win, A_BOLD);

        // Weekday headers
        int cell_width = width / 7;
        for (int i = 0; i < 7; i++) {
            mvwprintw(win, 3, i * cell_width, "%s", DAY_SHORTNAMES[i]);
        }
        wnoutrefresh(win);
    }
};
