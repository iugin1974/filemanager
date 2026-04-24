#include "panel_view.h"
#include "panel.h"
#include <string.h>
#include <ncurses.h>
#include "status_bar.h"

Panel_view::Panel_view(Panel& p, int height, int width, int x, int y)
  : panel(p),
	status_bar(y + height, width)
{
  win = newwin(height, width, x, y);

}



void Panel_view::draw() {
  int height, width;
  getmaxyx(win, height, width);

  // aggiorna offset
  int selected = panel.get_selected_index();
  if (selected < offset)
    offset = selected;
  if (selected >= offset + height)
    offset = selected - height + 1;

  werase(win);  // pulisce la finestra

  const auto& files = panel.get_files();
  int visible = std::min((int)files.size() - offset, height);

  for (int i = 0; i < visible; i++) {
    bool is_selected = (i + offset == selected);
    files[i + offset].print(win, i, is_selected);
  }
  std::string p = panel.get_current_path().string();
  status_bar.print_message(panel.get_current_path());
  wrefresh(win);
}

Panel& Panel_view::get_panel() const {
  return panel;
}

