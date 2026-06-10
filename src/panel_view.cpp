#include "panel_view.h"
#include "panel.h"
#include "status_bar.h"
#include <ncurses.h>
#include <string.h>

Panel_view::Panel_view(Panel *p, int height, int width, int row, int col, int n)
    : panel(p), status_bar(row + height, col, width) {
  win = newwin(height, width, row, col);
  panel_number = n;
}

void Panel_view::draw(bool sync_mode) {
  int height, width;
  getmaxyx(win, height, width);

  // aggiorna offset
  int selected = panel->get_selected_index();
  if (selected < offset)
    offset = selected;
  if (selected >= offset + height)
    offset = selected - height + 1;

  werase(win); // pulisce la finestra

  const auto &files = panel->get_file_list();
  int visible = std::min((int)files.size() - offset, height);

for (int i = 0; i < visible; i++) {
    bool is_selected = (i + offset == selected);
    std::lock_guard<std::mutex> lock(panel->get_mutex());
    panel->get_file_list().at(i + offset).print(win, i, is_selected, width,
                                                 sync_mode || panel->is_active());
}

  if (panel->get_file_list().size() > 0)
    status_bar.print_message(panel->get_current_path(),
                             panel->get_current_file(), files.size(),
                             selected + 1);
  else
    status_bar.print_message(panel->get_current_path());

  // disegna i bordi
  draw_border(width, height);
  wnoutrefresh(win);
}

void Panel_view::set_offset(int o) {
  int height = getmaxy(win);
  int max_offset = std::max(0, (int)(panel->get_file_list().size()) - height);
  offset = std::min(o, max_offset);
}

Panel *Panel_view::get_panel() const { return panel; }

void Panel_view::draw_border(int w, int h) {

  chtype ch = ACS_VLINE;

    for (int y = 0; y < h; y++) {
      mvwaddch(win, y, w - 1, ch);
    for (int y = 0; y < h; y++) {
      mvwaddch(win, y, 0, ch);
  }
    }
}
