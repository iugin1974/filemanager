#pragma once
#include "panel.h"
#include "file_comparator.h"
#include "view.h"
#include <array>
#include <queue>
#include <string>

// Gestisce l'input dell'utente e coordina Panel e View.
class Controller {
public:
  explicit Controller(View &view);
  void init();
  void on_resize();
  // Processa un tasto. Restituisce true se l'utente ha richiesto l'uscita.
  bool handle_key(int ch);

  // Attiva/disattiva la modalità sync: i due pannelli si muovono insieme.
  void set_sync(bool);

  // in sync_mode ordina i files diversi in cima alla finestra
  void sort_different(bool);

  // Elimina il file selezionato (o i file taggati). Se silent=true non chiede conferma.
  void delete_file(bool silent);

  // Elimina i files di backup con ~
  void delete_backup_files();

  // Copia il file selezionato (o i file taggati) nel pannello inattivo.
  void copy_file();

  // Crea una directory nella cartella corrente del pannello attivo.
  void mkdir(const std::string &);

  // Rinomina il file selezionato.
  void move_file(const std::string &);

  // Sposta il file selezionato (o i file taggati) nel pannello inattivo.
  void move_file();

  // Esegue un comando della shell
  void execute_command(const std::string &);

  // Cambia la directory corrente
  void change_dir(const std::string &);

  // Crea un file vuoto nella cartella corrente del pannello attivo.
  void touch(const std::string &);

  // Segnala al controller che deve uscire al prossimo ciclo.
  void exit_status();

  // Ricarica il contenuto di entrambi i pannelli dal filesystem.
  void reload_panels();

  // sincronizza due file in sync_mode:
  // se il file non esiste nell'altro pannello, viene copiato
  // altrimenti il file più recente sovrascrive il più vecchio
  void sync_file();

  // se sync mode è attivo, salta al prossimo file che differisce
  // tra i due pannelli.
  void jump_to_next_different();

  // chiama il ridisegno dei pannelli
  void draw_panels();

  // cancella la command_bar
  void clear_command_bar();

private:
  bool changed = true;
  void test();

  bool exit = false;
  FileComparator comparator;
  // --- Stato ---
  View &view;
  std::array<Panel, 2> panels;
  bool sync_mode = false;

  // --- Helpers ---
  void align_panels();
  void sync_index();
  int get_active_panel_index() const;
  int get_active_panel_index();
  int get_inactive_panel_index() const;
  int get_inactive_panel_index();
  const Panel &get_active_panel() const;
  Panel &get_active_panel();
  const Panel &get_inactive_panel() const;
  Panel &get_inactive_panel();
  void change_active_panel();
  void jump_to_file(char ch);

  // Esegue fn sul pannello attivo, o su entrambi se sync_mode è attivo.
  template <typename Fn> void for_active_panels(Fn fn);

  // --- Azioni ---
  void enter_pressed();
  void go_up();
  void go_left();
  void go_right();
  void go_first();
  void go_last();
  void move(int direction, int lines);
  void toggle_tag_file();

  // --- Comandi ---
  std::string get_command(CommandType c);
  void search_file(const std::string &name);
  void evaluate_command(const std::string &cmd);
  bool show_file_exists_popup(const std::filesystem::path &source,
                              const std::filesystem::path &dest);
  bool show_delete_file_popup(const std::filesystem::path &path);
  std::string file_info(const std::filesystem::path &p);
  void sync_partner(bool sync);
};
