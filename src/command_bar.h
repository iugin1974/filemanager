#pragma once
#include <ncurses.h>
#include <string>

class CommandBar {
  public:
    static const int INFO = 0;
    static const int ERROR = 1;
    
    CommandBar();
    ~CommandBar();
    void init_bar(int y, int w);
    void print_message(std::string, const int message_type);
    std::string get_command();
    void clear();

  private:
    WINDOW* win;
    int y_pos;
    int width;

};
