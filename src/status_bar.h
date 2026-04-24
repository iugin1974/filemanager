#pragma once
#include <ncurses.h>
#include <string>

class StatusBar {
  public:
    StatusBar(int y_pos, int width);
    void print_message(std::string);
    void clear();

  private:
    WINDOW* win;

};
