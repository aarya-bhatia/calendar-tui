#include "section.h"
#include "table.h"
#include "util.h"
#include <algorithm>
#include <map>
#include <time.h>
#include <vector>

class EventSection : public Section {

  struct Event {
    int id;
    std::string description;

    Event(int id, const std::string &s) : id(id), description(s) {}
  };

  TableSection &table;
  std::map<long, std::vector<Event>> events;

  static long get_map_key(struct tm &date) {
    struct tm t;
    memset(&t, 0, sizeof t);
    t.tm_year = date.tm_year;
    t.tm_mon = date.tm_mon;
    t.tm_mday = date.tm_mday;
    t.tm_isdst = -1;
    return mktime(&t);
  }

public:
  EventSection(int y, int x, int h, int w, const CalendarState &state,
               TableSection &table)
      : Section(y, x, h, w), table(table) {}

  void draw(const CalendarState &state) override {
    werase(win);
    struct tm t = table.get_selected_date();
    std::vector<Event> &v = events[get_map_key(t)];
    for (int i = 0; i < v.size(); i++) {
      log_printf("Drawing event %d...", v[i].id);
      mvwprintw(win, i, 0, "(%d) %s", v[i].id, v[i].description.c_str());
    }
    wnoutrefresh(win);
  }

  bool handle_input(int ch, CalendarState &state) override {
    if (ch == 'a') {
      add_event(table.get_selected_date(), "dummy event");
    }
    return true;
  }

  void add_event(struct tm t, std::string description) {
    long key = get_map_key(t);
    log_printf("Adding event to key %ld: %s", key, description.c_str());
    events[key].emplace_back(events[key].size() + 1, description);
  }

  void remove_event(struct tm t, int id) {
    std::vector<Event> &v = events[get_map_key(t)];
    for (auto it = v.begin(); it != v.end(); it++) {
      if (it->id == id) {
        it = v.erase(it);
        break;
      }
    }
    for (int i = 0; i < v.size(); i++) {
      v[i].id = i + 1;
    }
  }
};
