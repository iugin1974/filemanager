#include "panel.h"
#include <filesystem>
#include <vector>

void Panel::change_dir(const std::filesystem::path& path) {
  current_path = path;
  selected_index = 0;
  reload();
}

void Panel::reload() {
  file_list.clear();
  for (auto& entry : std::filesystem::directory_iterator(current_path)) {
    file_list.push_back(entry);
  }
}

int Panel::get_selected_index() const {
  return selected_index;
}

void Panel::move_up() {
  if (selected_index > 0) selected_index--;
}

void Panel::move_down() {
  if (selected_index < (int)file_list.size() -1) selected_index++;
}

const std::filesystem::path& Panel::get_current_path() const {
  return current_path;
}

const std::vector<FileEntry>& Panel::get_files() const {
  return file_list;
}

const FileEntry& Panel::get_file(int i) const {
  return file_list.at(i);
}

bool Panel::go_up() {
  if (current_path.has_parent_path()) {
    current_path = current_path.parent_path();
    change_dir(current_path);
    return true;
  }
  return false;
}
