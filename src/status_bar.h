#pragma once
#include <ncurses.h>
#include "file_entry.h"
class StatusBar {
  public:
    StatusBar() : win(nullptr) {}
    StatusBar(int y_pos, int x_pos, int width);
    void print_message(const FileEntry&);
    void clear();

  private:
    WINDOW* win;

};
