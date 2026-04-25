#pragma once
#include <ncurses.h>
#include <string>

class CommandBar {
  public:
    CommandBar() : win(nullptr) {}
    CommandBar(int y_pos, int width);
    ~CommandBar();
    void print_message(std::string);
    std::string get_command();
    void clear();

  private:
    WINDOW* win;

};
