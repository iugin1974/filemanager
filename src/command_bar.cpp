#include "command_bar.h"
#include <ncurses.h>
#include <string>

CommandBar::CommandBar(int y_pos, int width) {
  win = newwin(1, width, y_pos, 0);
}

CommandBar::~CommandBar() {
  if (win)
    delwin(win);
}

std::string CommandBar::get_command() {
  nocbreak();
  echo();

  std::string input;

  werase(win);
  mvwprintw(win, 0, 0, ":");
  wrefresh(win);

  int ch;
  while ((ch = wgetch(win)) != '\n') {

    if (ch >= 32 && ch <= 126) {
      input.push_back(static_cast<char>(ch));
    } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
      if (!input.empty()) {
        input.pop_back();
      }
    }

    werase(win);
    mvwprintw(win, 0, 1, "%s", input.c_str());
    wrefresh(win);
  }

  noecho();
  cbreak();

  return input;
}

void CommandBar::print_message(std::string msg, const int message_type) {
  werase(win);

  if (message_type == CommandBar::ERROR)
    wattron(win, COLOR_PAIR(9) | A_BOLD);
  mvwprintw(win, 0, 0, "%s", msg.c_str());

  wrefresh(win);
  wattroff(win, COLOR_PAIR(9) | A_BOLD);
}

void CommandBar::clear() { werase(win); }
