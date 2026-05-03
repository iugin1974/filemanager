#pragma once
#include <ncurses.h>
#include <string>

class CommandBar {
  public:
    static const int INFO = 0;
    static const int ERROR = 1;
    
    CommandBar() : win(nullptr) {}
    CommandBar(int y_pos, int width);
    ~CommandBar();
    void print_message(std::string, const int message_type);
    std::string get_command();
    void clear();

  private:
    WINDOW* win;

};
