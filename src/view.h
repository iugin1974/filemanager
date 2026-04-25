#pragma once

#include <ncurses.h>
#include "panel_view.h"
#include "panel.h"
#include "command_bar.h"

class View {
  public:
    View();
void init_panels(Panel*, Panel*);
void add_panel(Panel& p);
void draw_panels();
CommandBar& get_command_bar(int active_panel);

  private:
Panel_view panels[2];
};
