#pragma once
#include "panel.h"
#include "view.h"
#include <array>
#include <queue>
#include <string>

// Gestisce l'input dell'utente e coordina Panel e View.
class Controller {
public:
  explicit Controller(View &view);

  // Processa un tasto. Restituisce true se l'utente ha richiesto l'uscita.
  bool handle_key(int ch);

  // Attiva/disattiva la modalità sync: i due pannelli si muovono insieme.
  void set_sync(bool);

  // Elimina il file selezionato (o i file taggati). Se silent=true non chiede conferma.
  void delete_file(bool silent);

  // Copia il file selezionato (o i file taggati) nel pannello inattivo.
  void copy_file();

  // Crea una directory nella cartella corrente del pannello attivo.
  void mkdir(const std::string &);

  // Rinomina il file selezionato.
  void move_file(const std::string &);

  // Sposta il file selezionato (o i file taggati) nel pannello inattivo.
  void move_file();
  
  // Cambia la directory corrente
  void change_dir(const std::string &);

  // Crea un file vuoto nella cartella corrente del pannello attivo.
  void touch(const std::string &);

  // Segnala al controller che deve uscire al prossimo ciclo.
  void exit_status();

  // Ricarica il contenuto di entrambi i pannelli dal filesystem.
  void reload_panels();

private:
  void test();
  
  bool exit = false;

  // --- Stato ---
  View &view;
  std::array<Panel, 2> panels;
  bool sync_mode = false;

  // --- Helpers ---
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
  void enter_pressed(int selected_line, int panel_index);
  void go_up();
  void go_back();
  void move_up();
  void move_down();
  void sync_move(bool up);
  void toggle_tag_file();

  // --- Comandi ---
  std::string get_command();
  void evaluate_command(const std::string &cmd);
  bool show_file_exists_popup(const std::filesystem::path &source,
                              const std::filesystem::path &dest);
  bool show_delete_file_popup(const std::filesystem::path &path);
  std::string file_info(const std::filesystem::path &p);
  void sync_partner(bool sync);
};
