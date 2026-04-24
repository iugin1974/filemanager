#pragma once

#include <ncurses.h>
#include <vector>
#include "panel_view.h"
#include "panel.h"

class View {
  public:
void draw_panels();
void add_panel(Panel& p);

  private:
std::vector<Panel_view> panels;
};
