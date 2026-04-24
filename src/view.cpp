#include <ncurses.h>
#include "view.h"
#include "panel_view.h"
#include "panel.h"

void View::add_panel(Panel& p) {
  int l = 0, c = 0;
  getmaxyx(stdscr, l, c);
  int width = c / (panels.size() + 1);
  Panel_view pw(p, l - 2, width, 0, panels.size() * width);
  panels.push_back(pw);

}

void View::draw_panels() {
  for (int i = 0; i < panels.size(); i++) {
    panels.at(i).draw();
  }
}
