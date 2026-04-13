#pragma once
#include <vector>
#include <memory>
#include "calendar_state.h"
#include "section.h"

class Application {
    CalendarState state;
    std::vector<std::unique_ptr<Section>> sections;
    bool quit_signal = false;

public:
    Application();
    ~Application();

    void run();

private:
    void init_ncurses();
    void init_state();
    void recompute_layout();
};
