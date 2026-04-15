#include "section.h"
#include "calendar.h"
#include "util.h"
#include <algorithm>
#include <map>
#include <time.h>
#include <vector>

class EventPane : public Section {

  struct Event {
    int id;
    std::string description;

    Event(int id, const std::string &s);
  };

  Calendar &table;
  std::map<long, std::vector<Event>> events;

  static long get_map_key(struct tm &date);

public:
  EventPane(int y, int x, int h, int w, const CalendarState &state,
               Calendar &table);

  void draw(const CalendarState &state) override;

  bool handle_input(int ch, CalendarState &state) override;

  void add_event(struct tm t, std::string description);

  void remove_event(struct tm t, int id);
};
