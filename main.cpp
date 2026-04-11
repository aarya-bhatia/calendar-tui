#include <assert.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEBUG 1

#define log_printf(fmt, ...)                                                   \
  do {                                                                         \
    if (DEBUG)                                                                 \
      fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__,   \
              __VA_ARGS__);                                                    \
  } while (0)

#define log_println(str) log_printf("%s", str)

void draw() { doupdate(); }

int main(int argc, char *argv[]) {
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    perror("ioctl");
    return 1;
  }

  int N_WEEKS = 4;
  int N_DAYS_IN_WEEK = 7;

  int cell_height = w.ws_row / N_WEEKS;
  int cell_width = w.ws_col / N_DAYS_IN_WEEK;

  initscr();
  cbreak();
  refresh();
  keypad(stdscr, true);

  int logfile = open("app.log", O_CREAT | O_TRUNC | O_WRONLY, 0640);
  dup2(logfile, 2);
  close(logfile);

  log_printf("init screen with %d ln x %d col", LINES, COLS);
  log_printf("cell height:%d width:%d", cell_height, cell_width);

  noecho();
  curs_set(0);

  while (1) {
    draw();

    int ch = getch();
    if (ch == 'q') {
      break;
    } else if (ch == KEY_RIGHT || ch == 'l') {
    } else if (ch == KEY_LEFT || ch == 'h') {
    } else if (ch == KEY_UP || ch == 'k') {
    } else if (ch == KEY_DOWN || ch == 'j') {
    }
  }

  clear();
  refresh();
  getch();

  endwin();
}
