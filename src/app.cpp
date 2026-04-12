#include "app.h"
#include <ncurses.h>
#include <fcntl.h>
#include <unistd.h>
#include "header_section.h"
#include "grid.h"
#include "footer_section.h"
#include "util.h"

#define LOG_FILENAME "app.log"

#define SELECTION_COLOR 1
#define OTHER_MONTH_COLOR 2
#define FOOTER_COLOR 3

Application::Application() {
    init_ncurses();
    init_state();
    recompute_layout();
}

Application::~Application() {
    endwin();
}

void Application::init_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    use_default_colors();
    init_pair(SELECTION_COLOR, COLOR_RED, -1);
    init_pair(OTHER_MONTH_COLOR, COLOR_BLUE, -1);
    
    // Use index 236 (Dark Grey) if supported, otherwise fallback to COLOR_BLACK
    if (COLORS >= 256) {
        init_pair(FOOTER_COLOR, COLOR_WHITE, 236);
    } else {
        init_pair(FOOTER_COLOR, COLOR_WHITE, COLOR_BLACK);
    }
    
    int logfile = open(LOG_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0640);
    dup2(logfile, 2);
    close(logfile);
}

void Application::init_state() {
    time_t raw;
    time(&raw);
    localtime_r(&raw, &state.today);
    state.selected = state.today;
    state.selected.tm_hour = 12;
    state.selected.tm_isdst = -1;
    mktime(&state.selected);
}

void Application::recompute_layout() {
    sections.clear();
    erase();
    refresh();

    if (LINES < 10 || COLS < 20) return;

    int header_h = 4;
    int footer_h = 1;
    int grid_h = LINES - header_h - footer_h;

    sections.push_back(std::make_unique<HeaderSection>(0, header_h, COLS));
    sections.push_back(std::make_unique<GridSection>(header_h, grid_h, COLS, state));
    sections.push_back(std::make_unique<FooterSection>(header_h + grid_h, footer_h, COLS));
}

void Application::run() {
    while (true) {
        for (auto& s : sections) {
            s->draw(state);
        }
        doupdate();

        int ch = getch();
        if (ch == 'q') break;
        if (ch == KEY_RESIZE) {
            recompute_layout();
            continue;
        }

        for (auto& s : sections) {
            if (s->handle_input(ch, state)) {
                break;
            }
        }
    }
}
