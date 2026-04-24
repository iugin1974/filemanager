#include "status_bar.h"
#include <ncurses.h>
#include <string>

StatusBar::StatusBar(int y_pos, int width) {
  win = newwin(1, width, y_pos, 0);
  wattron(win, A_REVERSE | A_BOLD);
}

void StatusBar::print_message(std::string msg) {
    werase(win);

    wattron(win, A_REVERSE);

    // riempi tutta la riga di spazi invertiti
    whline(win, ' ', getmaxx(win));

    // scrivi il messaggio sopra
    mvwprintw(win, 0, 0, "%s", msg.c_str());

    wattroff(win, A_REVERSE);

    wrefresh(win);
}

void StatusBar::clear() {
  werase(win);
}
