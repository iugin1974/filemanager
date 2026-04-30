#include <ncurses.h>
#include <memory>
#include "view.h"
#include "panel_view.h"
#include "panel.h"

View::View(){}

void View::init_panels(Panel* left, Panel* right) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int width = cols / 2;
    panels[0] = std::make_unique<Panel_view>(left,  rows - 2, width, 0, 0,     0);
    panels[1] = std::make_unique<Panel_view>(right, rows - 2, width, 0, width, 1);
}

void View::draw_panels() {
  for (int i = 0; i < 2; i++) {
    panels[i]->draw();
  }
}

void View::set_offset(const Panel& p, int o) {
  for (int i = 0; i < 2; i++) {
      if (panels[i]->get_panel() == &p) {
       panels[i]->set_offset(o);
       break;
      }
  }
}

CommandBar& View::get_command_bar(int active_panel) {
return panels[active_panel]->get_command_bar();
}
