#include "panel_view.h"
#include "panel.h"
#include <string.h>
#include <ncurses.h>
#include "status_bar.h"

Panel_view::Panel_view(Panel* p, int height, int width, int row, int col, int n)
  : panel(p),
    status_bar(row + height, col, width),
    command_bar(row + height + 1, width)
{
    win = newwin(height, width, row, col);
    panel_number = n;
}

void Panel_view::draw() {
  int height, width;
  getmaxyx(win, height, width);

  // aggiorna offset
  int selected = panel->get_selected_index();
  if (selected < offset)
    offset = selected;
  if (selected >= offset + height)
    offset = selected - height + 1;

  wclear(win);  // pulisce la finestra

  const auto& files = panel->get_raw_file_list();
  int visible = std::min((int)files.size() - offset, height);

  for (int i = 0; i < visible; i++) {
    bool is_selected = (i + offset == selected);
    files[i + offset].print(win, i, is_selected);
  }
  
  if (panel->get_raw_file_list().size() > 0)
    status_bar.print_message(panel->get_current_path(), panel->get_current_file());
  else
    status_bar.print_message(panel->get_current_path());
    
  // disegna i bordi
 draw_border(width, height); 
  wrefresh(win);
}

void Panel_view::set_offset(int o) {
 int height = getmaxy(win); 
  int max_offset = std::max(0, (int)(panel->get_raw_file_list().size()) - height);
offset = std::min(o, max_offset);
}

CommandBar& Panel_view::get_command_bar() {
return command_bar;
}

Panel* Panel_view::get_panel() const {
  return panel;
}

void Panel_view::draw_border(int w, int h) {

    char ch = '|';

    if (panel->is_active()) wattron(win, A_REVERSE);

    if (panel_number == 0) {
        // solo bordo destro
        for (int y = 0; y < h; y++) {
            mvwaddch(win, y, w - 1, ch);
        }
    } 
    else if (panel_number == 1) {
        // solo bordo sinistro
        for (int y = 0; y < h; y++) {
            mvwaddch(win, y, 0, ch);
        }
    }

    wattroff(win, A_REVERSE);
}
