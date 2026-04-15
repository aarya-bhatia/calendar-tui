#include "event_pane.h"

EventPane::Event::Event(int id, const std::string &s) : id(id), description(s) {}

long EventPane::get_map_key(struct tm &date) {
  struct tm t;
  memset(&t, 0, sizeof t);
  t.tm_year = date.tm_year;
  t.tm_mon = date.tm_mon;
  t.tm_mday = date.tm_mday;
  t.tm_isdst = -1;
  return mktime(&t);
}

EventPane::EventPane(int y, int x, int h, int w, const CalendarState &state,
             Calendar &table)
    : Section(y, x, h, w), table(table) {}

void EventPane::draw(const CalendarState &state) {
  werase(win);
  struct tm t = table.get_selected_date();
  std::vector<Event> &v = events[get_map_key(t)];
  for (int i = 0; i < (int)v.size(); i++) {
    log_printf("Drawing event %d...", v[i].id);
    mvwprintw(win, i, 0, "(%d) %s", v[i].id, v[i].description.c_str());
  }
  wnoutrefresh(win);
}

bool EventPane::handle_input(int ch, CalendarState &state) {
  if (ch == 'a') {
    add_event(table.get_selected_date(), "dummy event");
  }
  return true;
}

void EventPane::add_event(struct tm t, std::string description) {
  long key = get_map_key(t);
  log_printf("Adding event to key %ld: %s", key, description.c_str());
  events[key].emplace_back(events[key].size() + 1, description);
}

void EventPane::remove_event(struct tm t, int id) {
  std::vector<Event> &v = events[get_map_key(t)];
  for (auto it = v.begin(); it != v.end(); it++) {
    if (it->id == id) {
      it = v.erase(it);
      break;
    }
  }
  for (int i = 0; i < (int)v.size(); i++) {
    v[i].id = i + 1;
  }
}
