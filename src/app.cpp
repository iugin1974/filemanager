#include "app.h"
#include "controller.h"
#include "ncurses.h"
#include "view.h"
#include <signal.h>

static volatile sig_atomic_t resized = 0;

void sigwinch_handler(int) { resized = 1; }

void App::run() {
  initscr();            // inizializza ncurses
  cbreak();             // input carattere per carattere senza Enter
  noecho();             // non mostrare i tasti premuti
  keypad(stdscr, TRUE); // abilita i tasti freccia
  start_color();
  use_default_colors();

  signal(SIGWINCH, sigwinch_handler);

  init_pair(1, COLOR_BLUE, -1);         // directory
  init_pair(2, COLOR_GREEN, -1);        // eseguibile
  init_pair(3, COLOR_BLACK, -1);        // file normale
  init_pair(4, COLOR_RED, -1);          // file compresso (.zip, .tar, ecc)
  init_pair(5, COLOR_YELLOW, -1);       // file multimediale
  init_pair(6, COLOR_WHITE, -1);        // file normale
  init_pair(7, -1, COLOR_WHITE);        // selezione reverse
  init_pair(8, -1, COLOR_BLUE);         // pannello attivo
  init_pair(9, COLOR_WHITE, COLOR_RED); // messaggio errore

  refresh();

  View view;
  Controller controller(view);

  timeout(100);
  int ch;
  while (true) {
    if (resized) {
      resized = false;
      endwin();
      refresh();
      controller.on_resize();
    }
    ch = getch();
    if (ch == ERR) {
     controller.draw_panels();
     continue;
    }
    bool exit = controller.handle_key(ch);
    if (exit)
      break;
  }
  endwin();
}
