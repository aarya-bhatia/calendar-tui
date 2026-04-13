#pragma once
#include "section.h"
#include "cell.h"
#include <vector>
#include <memory>

class GridSection : public Section {
    std::vector<std::unique_ptr<Cell>> cells;
    struct tm anchor_date; // Always Sunday of the first row

public:
    GridSection(int y, int x, int h, int w, const CalendarState& state);
    
    void draw(const CalendarState& state) override;
    bool handle_input(int ch, CalendarState& state) override;
    
    void sync_dates(const CalendarState& state);
};
