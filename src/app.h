#pragma once

#include "view.h"
#include <memory>
#include <ncurses.h>
#include <stdio.h>
#include <vector>

#define LOG_FILENAME "app.log"
#define MIN_LINES 20
#define MIN_COLS 50

class Application {
  AppState state;
  std::vector<std::unique_ptr<View>> views;

public:
  Application();
  ~Application();

  void run();
  void render();
  void handle_input();
  void handle_input_typing(int);
  void handle_input_actions(int);

private:
  void recompute_layout();
};
