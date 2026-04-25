#pragma once

#include "panel.h"
#include <ncurses.h>
#include "status_bar.h"
#include "command_bar.h"

class Panel_view {
  public:
Panel_view() : panel(nullptr), win(nullptr) {}
    Panel_view(Panel* panel, int height, int width, int row, int col, int n);
    void draw();
    Panel* get_panel() const;
    CommandBar& get_command_bar();

  private:
    int panel_number;
    int get_offset();
    void draw_border(int, int);
    Panel* panel;
    StatusBar status_bar;
    CommandBar command_bar;
    WINDOW* win;
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;
    int offset = 0;
};
