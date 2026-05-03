#include "app.h"
#include "view.h"
#include "ncurses.h"
#include "controller.h"

void App::run() {
  initscr();        // inizializza ncurses
  cbreak();         // input carattere per carattere senza Enter
  noecho();         // non mostrare i tasti premuti
  keypad(stdscr, TRUE);  // abilita i tasti freccia
start_color();
use_default_colors();
init_pair(1, COLOR_BLUE, -1);    // directory
init_pair(2, COLOR_GREEN, -1);   // eseguibile
init_pair(3, COLOR_CYAN, -1);    // link simbolico
init_pair(4, COLOR_RED, -1);     // file compresso (.zip, .tar, ecc)
init_pair(5, COLOR_YELLOW, -1);  // file multimediale
init_pair(6, COLOR_WHITE, -1);   // file normale
init_pair(7, -1, COLOR_WHITE);   // selezione reverse
init_pair(8, -1, COLOR_BLUE);    // pannello attivo
init_pair(9, COLOR_WHITE, COLOR_RED);     // messaggio errore

  refresh();

  View view;
  Controller controller(view);

  int ch;
  while ((ch = getch())) {
   bool exit = controller.handle_key(ch);
   if (exit) break;
  }
  endwin();
}

