#pragma once
#include "history.h"
#include <filesystem>
#include <vector>
#include "file_entry.h"
#include <stack>
#include <mutex>

class Panel {
public:
  Panel(int);
  void change_dir(const std::filesystem::path& path);
  void set_sync_partner(Panel* p);
  // crea la aligned_file_list, inserendo FileEntry vuoti come
  // placeholder per i buchi nella lista, quando mancano file presenti
  // nell'altro pannello.
  void align_with(std::vector<FileEntry>& other_file_list);
  // Ricarica la raw_list
  void reload();
  // Restituisce la lista da visualizzare: la raw_list se sync_mode non
  // è attivo, altrimenti la aligned_list.
  const std::vector<FileEntry>& get_file_list() const;
  std::vector<FileEntry>& get_file_list();
  const std::filesystem::path& get_current_path() const;
  std::string get_current_file_name() const;
  const std::vector<FileEntry>& get_raw_file_list() const;
  std::vector<FileEntry>& get_raw_file_list();
  std::filesystem::path get_current_file_fullpath() const;
  Panel* get_aligned_panel();
  int get_selected_index() const;
  int get_diff_files() const;
  void set_selected_index(int i);
  void move_up(int lines);
  void move_down(int lines);
  const FileEntry& get_file(int) const;
  bool go_up();
  bool go_left();
  bool go_right();
  void select_first();
  void select_last();
  bool is_active() const;
  void set_active(bool);
  int contains(const std::string& name) const;
  void show_hidden_files(bool show);
  bool is_showing_hidden() const;
  FileEntry& get_file_at(int i);
  FileEntry& get_current_file();
  const FileEntry& get_current_file() const;
  void update_selected_index();
  const std::vector<FileEntry>& get_tagged_files() const;
  void tag_current_file(bool t);
  void toggle_tag_current_file();
  void clear_tagged_selection();
  std::vector<FileEntry> get_files_to_operate() const;
  std::mutex& get_mutex();
  
private:
  Panel* sync_partner = nullptr;
  std::mutex mutex;
  History history;
  std::filesystem::path current_path;
  std::vector<FileEntry> raw_file_list;
  std::vector<FileEntry> aligned_file_list;
  std::vector<FileEntry> tagged_files;
  int selected_index = 0;
  int diff_files = -1;
  bool active = false;
  bool has_sync_partner() const;
  void reset_diff_files();
  bool show_hidden = false;
  int id;
};
