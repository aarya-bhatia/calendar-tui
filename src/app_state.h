#pragma once
#include <map>
#include <string>
#include <vector>
#include <time.h>

#define DATA_DIRECTORY "data/"

struct AppState {
  AppState();
  ~AppState();

  // Public Action API
  enum MoveDir { LEFT, RIGHT, TOP, BOTTOM };
  void move(MoveDir dir);
  void jump_to_date(struct tm date);
  void insert_event(const std::string& description);
  void quit() { running = false; }

  // Read-only state access for View
  bool is_running() const { return running; }
  bool is_typing() const { return typing; }
  struct tm get_view_month() const { return first_day_of_view_month; }
  struct tm get_today() const { return today; }
  int get_selected_index() const { return selected_entry_index; }
  
  struct Event {
    int id;
    std::string description;
    Event(int id, const std::string &s) : id(id), description(s) {}
  };
  const std::vector<Event>& get_events(const struct tm &date) const;

  void load_events();
  void save_event(struct tm date, Event &e);

  // Constants
  static const int CALENDAR_NUM_ROWS = 6;
  static const int CALENDAR_NUM_COLS = 7;
  static const int CALENDAR_NUM_ENTRIES = CALENDAR_NUM_ROWS * CALENDAR_NUM_COLS;

  struct tm get_entry_date(int index) const;
  struct tm get_selected_date() const;

private:
  bool typing = false;
  bool running = true;
  struct tm first_day_of_view_month;
  struct tm today;
  struct tm anchor_tm;
  int selected_entry_index = 0;

  std::map<long, std::vector<Event>> events;

  // Internal Helpers
  void refresh_today_date();
  void update_view_date(struct tm day_in_view_month);
  void set_view_prev_month();
  void set_view_next_month();
  void update_selection(struct tm new_selection_date);
  
  int get_today_entry_index(struct tm today) const;
  int get_entry_index(struct tm date) const;
  struct tm get_anchor_date(int view_year, int view_month) const;
  
  static long get_map_key(const struct tm &date);
  void add_event(struct tm t, std::string description);
  void remove_event(struct tm t, int id);
};
