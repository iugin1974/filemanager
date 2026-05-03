#pragma once

#include "panel.h"
#include "view.h"
#include <array>
#include <queue>
#include <string>

class Controller {
public:
  explicit Controller(View &view);

  bool handle_key(int ch);

  void set_sync(bool);
  void delete_file(bool silent);
  void copy_file();
  void mkdir(const std::string &);
  void move_file(const std::string &);
  void move_file();
  void touch(const std::string &);
  void exit_status();
  void reload_panels();

private:
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
};
