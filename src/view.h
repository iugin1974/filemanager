#pragma once

#include "command_bar.h"
#include "panel.h"
#include "panel_view.h"
#include <memory>
#include <ncurses.h>

class View {
public:
  View();
  void init_panels(Panel *, Panel *);
  void add_panel(Panel &p);
  void draw_panels();
  CommandBar &get_command_bar();
  void set_offset(const Panel &p, int o);

private:
  std::unique_ptr<Panel_view> panels[2];
  int height = 0;
  int width = 0;
  CommandBar command_bar;
};
