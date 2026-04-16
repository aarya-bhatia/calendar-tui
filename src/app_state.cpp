#include "app_state.h"
#include "util.h"
#include <dirent.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <string.h>
#include <string>

AppState::AppState() {
  refresh_today_date();
  update_view_date(today);

  anchor_tm = get_anchor_date(first_day_of_view_month.tm_year,
                              first_day_of_view_month.tm_mon);
  selected_entry_index = get_today_entry_index(today);
}

AppState::~AppState() {}

void AppState::move(MoveDir dir) {
  if (dir == RIGHT) {
    selected_entry_index++;
  } else if (dir == LEFT) {
    selected_entry_index--;
  } else if (dir == TOP) {
    selected_entry_index -= 7;
  } else if (dir == BOTTOM) {
    selected_entry_index += 7;
  }

  if (selected_entry_index < 0) {
    selected_entry_index += AppState::CALENDAR_NUM_ENTRIES;
    anchor_tm.tm_mday -= AppState::CALENDAR_NUM_ENTRIES;
    mktime(&anchor_tm);
    set_view_prev_month();
  } else if (selected_entry_index >= AppState::CALENDAR_NUM_ENTRIES) {
    selected_entry_index -= AppState::CALENDAR_NUM_ENTRIES;
    anchor_tm.tm_mday += AppState::CALENDAR_NUM_ENTRIES;
    mktime(&anchor_tm);
    set_view_next_month();
  }
}

void AppState::jump_to_date(struct tm date) { update_selection(date); }

void AppState::insert_event(const std::string &description) {
  add_event(get_selected_date(), description);
}

void AppState::update_selection(struct tm new_selection_date) {
  struct tm new_anchor =
      get_anchor_date(new_selection_date.tm_year, new_selection_date.tm_mon);
  if (new_anchor.tm_year != anchor_tm.tm_year ||
      new_anchor.tm_mon != anchor_tm.tm_mon) {
    anchor_tm = new_anchor;
    update_view_date(new_selection_date);
  }
  this->selected_entry_index = get_entry_index(new_selection_date);
}

int AppState::get_entry_index(struct tm date) const {
  struct tm tmcopy = anchor_tm;
  time_t t1 = mktime(&tmcopy);
  time_t t2 = mktime(&date);
  return abs(t2 - t1) / SECONDS_IN_DAY;
}

struct tm AppState::get_entry_date(int index) const {
  struct tm entry_date = anchor_tm;
  entry_date.tm_mday += index;
  mktime(&entry_date);
  return entry_date;
}

struct tm AppState::get_anchor_date(int view_year, int view_month) const {
  struct tm t;
  memset(&t, 0, sizeof t);
  t.tm_year = view_year;
  t.tm_mon = view_month;
  t.tm_mday = 1;
  t.tm_isdst = -1;
  mktime(&t);
  t.tm_mday -= t.tm_wday;
  mktime(&t);
  return t;
}

int AppState::get_today_entry_index(struct tm today) const {
  struct tm tmcopy = anchor_tm;
  time_t t1 = mktime(&tmcopy);
  time_t t2 = mktime(&today);
  return abs(t2 - t1) / SECONDS_IN_DAY;
}

struct tm AppState::get_selected_date() const {
  struct tm t = anchor_tm;
  t.tm_mday += selected_entry_index;
  mktime(&t);
  return t;
}

void AppState::refresh_today_date() {
  time_t raw;
  time(&raw);
  memset(&today, 0, sizeof today);
  localtime_r(&raw, &today);
}

void AppState::update_view_date(struct tm day_in_view_month) {
  struct tm t;
  memset(&t, 0, sizeof t);
  t.tm_mday = 1;
  t.tm_year = day_in_view_month.tm_year;
  t.tm_mon = day_in_view_month.tm_mon;
  mktime(&t);
  first_day_of_view_month = t;
}

void AppState::set_view_prev_month() {
  first_day_of_view_month.tm_mon--;
  mktime(&first_day_of_view_month);
}

void AppState::set_view_next_month() {
  first_day_of_view_month.tm_mon++;
  mktime(&first_day_of_view_month);
}

long AppState::get_map_key(const struct tm &date) {
  struct tm t;
  memset(&t, 0, sizeof t);
  t.tm_year = date.tm_year;
  t.tm_mon = date.tm_mon;
  t.tm_mday = date.tm_mday;
  t.tm_isdst = -1;
  return mktime(&t);
}

void AppState::add_event(struct tm t, std::string description) {
  long key = get_map_key(t);
  events[key].emplace_back(events[key].size() + 1, description);
  save_event(t, events[key].back());
}

void AppState::remove_event(struct tm t, int id) {
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

const std::vector<AppState::Event> &
AppState::get_events(const struct tm &date) const {
  static const std::vector<Event> empty;
  long key = get_map_key(date);
  auto it = events.find(key);
  if (it == events.end())
    return empty;
  return it->second;
}

void AppState::save_event(struct tm date, Event &e) {
  long key = AppState::get_map_key(date);
  time_t timestamp;
  time(&timestamp);
  std::string filename = std::string(DATA_DIRECTORY) + "/" +
                         std::to_string(key) + "_" + std::to_string(timestamp);

  std::ofstream file(filename);

  if (file.is_open()) {
    file << e.description << std::endl;
    file.close();
    log_printf("saved event to file %s", filename.c_str());
  } else {
    log_printf("failed to open file %s", filename.c_str());
  }
}

void AppState::load_events() {
  DIR *dirp = opendir(DATA_DIRECTORY);
  if (!dirp) {
    perror("opendir");
    return;
  }
  struct dirent *entry;
  std::regex pattern("(\\d+)_\\d");
  while ((entry = readdir(dirp)) != NULL) {
    const char *filename = entry->d_name;
    std::string filename_s = std::string(filename);
    std::smatch matches;
    if (std::regex_search(filename_s, matches, pattern) && matches.size() > 1) {
      std::string key_s = matches.str(1);
      long key = std::stol(key_s);
      std::ifstream file(std::string(DATA_DIRECTORY) + "/" + filename_s);
      std::stringstream buffer;
      buffer << file.rdbuf();
      std::string description = buffer.str();
      if (description.size() > 0 &&
          description[description.size() - 1] == '\n') {
        description.pop_back();
      }
      log_printf("Loaded event %ld: %s", key, description.c_str());
      events[key].emplace_back(events[key].size() + 1, description);
    }
  }
  closedir(dirp);
}
