// popup.cpp
#include "popup.h"
#include <ncurses.h>
#include <string>
#include <vector>

int Popup::show(std::vector<std::string> lines, std::vector<std::string> keys) {
    // calcola dimensioni
    int width = 0;
    for (const auto& l : lines)
        width = std::max(width, (int)l.size());
    for (const auto& k : keys) {
        width = std::max(width, (int)k.size());
     width += 3;   
    }
    width += 20;  // margini

    int height = lines.size() + 4;  // righe + separatore tasti + bordi

    // centra nella console
    int y = (LINES - height) / 2;
    int x = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, y, x);
    box(win, 0, 0);

    // stampa le righe
    for (int i = 0; i < (int)lines.size(); i++)
        mvwprintw(win, i + 1, 2, "%s", lines[i].c_str());

    // separatore
    mvwhline(win, lines.size() + 1, 1, ACS_HLINE, width - 2);

    // stampa i tasti in basso centrati
    int key_x = 2;
    for (const auto& k : keys) {
        wattron(win, A_BOLD);
        mvwprintw(win, lines.size() + 2, key_x, "%s", k.c_str());
        wattroff(win, A_BOLD);
        key_x += k.size() + 2;
    }

    wrefresh(win);
int ch = wgetch(win);
delwin(win);

for (int i = 0; i < (int)keys.size(); i++) {
    // prende il carattere dentro le parentesi quadre
    char key = tolower(keys[i][1]);
    if (tolower(ch) == key)
        return i;
}
return -1;  // tasto non riconosciuto}
}
