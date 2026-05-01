#pragma once
#include <filesystem>
#include <vector>
#include "file_entry.h"
#include <stack>

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
  bool go_back();
  bool is_active() const;
  void set_active(bool);
  const int contains(const std::string& name) const;
  void show_hidden_files(bool show);
  bool is_showing_hidden() const;
  FileEntry& get_file_at(int i);
  FileEntry& get_current_file();
  const std::vector<std::filesystem::path>& get_tagged_files() const;
  void tag_current_file(bool t);
  void toggle_tag_current_file();
  std::vector<std::filesystem::path> get_files_to_operate() const;
private:
  std::stack<std::filesystem::path> path_history;
  std::filesystem::path current_path;
  std::vector<FileEntry> file_list;
  std::vector<std::filesystem::path> tagged_files;
  int selected_index = 0;
  bool active = false;
  void update_selected_index();
  bool show_hidden = false;
};
