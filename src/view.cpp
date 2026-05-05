#include "view.h"
#include "command_bar.h"
#include "panel.h"
#include "panel_view.h"
#include <memory>
#include <ncurses.h>

View::View() {
  getmaxyx(stdscr, height, width);
  command_bar.init_bar(height - 1, width);
}

void View::init_panels(Panel *left, Panel *right) {
  panels[0] = std::make_unique<Panel_view>(left, height - 2, width / 2, 0, 0, 0);
  panels[1] = std::make_unique<Panel_view>(right, height - 2, width / 2, 0, width / 2, 1);
}

void View::draw_panels() {
  for (int i = 0; i < 2; i++) {
    panels[i]->draw();
  }
}

void View::set_offset(const Panel &p, int o) {
  for (int i = 0; i < 2; i++) {
    if (panels[i]->get_panel() == &p) {
      panels[i]->set_offset(o);
      break;
    }
  }
}

CommandBar &View::get_command_bar() {
  return command_bar;
}
