#include "grid.h"
#include "calendar_state.h"
#include <cstring>

GridSection::GridSection(int y, int x, int h, int w, const CalendarState& state) : Section(y, x, h, w) {
    int cell_h = h / 6;
    int cell_w = w / 7;

    for (int i = 0; i < 42; i++) {
        int ry = i / 7;
        int rx = i % 7;
        cells.emplace_back(std::make_unique<Cell>(win, cell_h, cell_w, ry * cell_h, rx * cell_w));
    }

    // Initialize anchor_date based on state.selected
    memcpy(&anchor_date, &state.selected, sizeof(struct tm));
    anchor_date.tm_mday = 1;
    anchor_date.tm_hour = 12;
    anchor_date.tm_isdst = -1;
    mktime(&anchor_date); // Normalize to get wday
    
    anchor_date.tm_mday -= anchor_date.tm_wday;
    mktime(&anchor_date); // Anchor is now the Sunday before the 1st
    
    sync_dates(state);
}

void GridSection::sync_dates(const CalendarState& state) {
    struct tm curr = anchor_date;
    for (int i = 0; i < 42; i++) {
        curr.tm_isdst = -1;
        mktime(&curr);
        cells[i]->date = curr;
        curr.tm_mday++;
    }
}

void GridSection::draw(const CalendarState& state) {
    for (int i = 0; i < 42; i++) {
        bool selected = (cells[i]->date.tm_mday == state.selected.tm_mday &&
                        cells[i]->date.tm_mon == state.selected.tm_mon &&
                        cells[i]->date.tm_year == state.selected.tm_year);
        
        bool today = (cells[i]->date.tm_mday == state.today.tm_mday &&
                      cells[i]->date.tm_mon == state.today.tm_mon &&
                      cells[i]->date.tm_year == state.today.tm_year);

        bool greyed = (cells[i]->date.tm_mon != state.selected.tm_mon);

        cells[i]->draw(selected, today, greyed);
    }
    wnoutrefresh(win);
}

bool GridSection::handle_input(int ch, CalendarState& state) {
    struct tm next = state.selected;
    next.tm_hour = 12;
    next.tm_isdst = -1;
    bool moved = false;

    if (ch == KEY_RIGHT || ch == 'l') {
        next.tm_mday++; moved = true;
    } else if (ch == KEY_LEFT || ch == 'h') {
        next.tm_mday--; moved = true;
    } else if (ch == KEY_UP || ch == 'k') {
        next.tm_mday -= 7; moved = true;
    } else if (ch == KEY_DOWN || ch == 'j') {
        next.tm_mday += 7; moved = true;
    }

    if (moved) {
        mktime(&next);
        state.selected = next;

        // Paged Scrolling Check
        // If we moved off the current grid view, shift anchor by 42 days
        time_t t_next = mktime(&next);
        time_t t_start = mktime(&cells[0]->date);
        time_t t_end = mktime(&cells[41]->date);

        if (t_next < t_start) {
            anchor_date.tm_mday -= 42;
            mktime(&anchor_date);
            sync_dates(state);
        } else if (t_next > t_end) {
            anchor_date.tm_mday += 42;
            mktime(&anchor_date);
            sync_dates(state);
        }
        return true;
    }

    return false;
}
