#include "panel.h"
#include <filesystem>
#include <vector>

Panel::Panel() {
current_path = std::getenv("HOME");
}

void Panel::change_dir(const std::filesystem::path& path) {
  current_path = path;
  selected_index = 0;
  reload();
}

void Panel::reload() {
    file_list.clear();

    for (auto& entry : std::filesystem::directory_iterator(current_path)) {

        // nome file
        const auto name = entry.path().filename().string();

        // escludi file nascosti (Linux/macOS: iniziano con '.')
        if (!show_hidden && !name.empty() && name[0] == '.') {
            continue;
        }

        file_list.push_back(entry);
    }

    update_selected_index();
}

void Panel::show_hidden_files(bool h) {
show_hidden = h;  
}

int Panel::get_selected_index() const {
  return selected_index;
}

void Panel::set_selected_index(int i) {
selected_index = i;
}

void Panel::update_selected_index() {
if (selected_index >= file_list.size()) selected_index = file_list.size() - 1;
if (selected_index == -1) selected_index = 0;
}

std::string Panel::get_current_file_name() const {
return file_list.at(selected_index).get_name();
}

std::filesystem::path Panel::get_current_file_fullpath() const {
return file_list.at(selected_index).get_path();  
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

bool Panel::is_active() const {
  return active;
}

void Panel::set_active(bool a) {
  active = a;
}

const int Panel::contains(const std::string& name) const {
for (int i = 0; i < file_list.size(); i++) {
if (file_list.at(i).get_name() == name) return i;
}
return -1;
}
