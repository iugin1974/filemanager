#pragma once

#include "panel.h"
#include <ncurses.h>
#include "status_bar.h"

class Panel_view {
  public:
    Panel_view(Panel& panel, int height, int width, int x, int y);
    void draw();
    Panel& get_panel() const;

  private:
    int get_offset();
    Panel& panel;
    StatusBar status_bar;
    WINDOW* win;
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;
    int offset = 0;
};
