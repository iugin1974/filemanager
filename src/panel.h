#pragma once
#include <filesystem>
#include <vector>
#include "file_entry.h"

class Panel {
public:
  Panel();
  void change_dir(const std::filesystem::path& path);
  void reload();
  const std::filesystem::path& get_current_path() const;
  std::string get_current_file_name() const;
  const std::vector<FileEntry>& get_files() const;
  std::filesystem::path get_current_file_fullpath() const;
  int get_selected_index() const;
  void set_selected_index(int i);
  void move_up();
  void move_down();
  const FileEntry& get_file(int) const;
  bool go_up();
  bool is_active() const;
  void set_active(bool);
  const int contains(const std::string& name) const;
  void show_hidden_files(bool show);
private:
  std::filesystem::path current_path;
  std::vector<FileEntry> file_list;
  int selected_index = 0;
  bool active = false;
  void update_selected_index();
  bool show_hidden = false;
};
