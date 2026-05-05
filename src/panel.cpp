#include "panel.h"
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <stack>
#include <vector>

Panel::Panel() { current_path = std::getenv("HOME"); }

void Panel::set_sync_partner(Panel *p) {
  sync_partner = p;
  if (p == nullptr)
    aligned_file_list.clear();
}

void Panel::change_dir(const std::filesystem::path &path) {
  current_path = path;
  selected_index = 0;
  reload();
}

const std::vector<FileEntry> &Panel::get_file_list() const {
  return has_sync_partner() ? aligned_file_list : raw_file_list;
}

#include <iostream>
void Panel::align_with(const std::vector<FileEntry> &other_file_list) {
  aligned_file_list.clear();
  std::cerr << "align_with\n";
  size_t i = 0, j = 0;
  while (i < raw_file_list.size() && j < other_file_list.size()) {
    std::cerr << "i: " << i << " [" << raw_file_list[i].get_name() << "]"
          << " - j: " << j << " [" << other_file_list[j].get_name() << "]\n";
    int cmp = raw_file_list[i].get_name().compare(other_file_list[j].get_name());
    if (cmp == 0) {
      aligned_file_list.push_back(raw_file_list[i]);
      ++i;
      ++j;
    } else if (cmp < 0) {
      aligned_file_list.push_back(raw_file_list[i]);
      ++i;
    } else {
      aligned_file_list.push_back(FileEntry()); // buco
      ++j;
    }
  }
  // aggiunge i files restanti
  while (i < raw_file_list.size())
    aligned_file_list.push_back(raw_file_list[i++]);
  while (j < other_file_list.size()) {
    aligned_file_list.push_back(FileEntry());
    j++;
  }
}

void Panel::reload() {
  raw_file_list.clear();
  try {
    for (auto &entry : std::filesystem::directory_iterator(current_path)) {
      const auto name = entry.path().filename().string();
      if (!show_hidden && !name.empty() && name[0] == '.')
        continue;
      raw_file_list.push_back(entry);
    }
  } catch (const std::filesystem::filesystem_error &e) {
    return;
  }

  std::sort(raw_file_list.begin(), raw_file_list.end(),
            [](const auto &a, const auto &b) {
              auto category = [](const auto &entry) {
                const std::string name =
                    entry.get_path().filename().string();
                bool hidden = !name.empty() && name[0] == '.';
                bool dir = entry.is_directory();
                if (hidden && dir)
                  return 0;
                if (!hidden && dir)
                  return 1;
                if (hidden && !dir)
                  return 2;
                return 3;
              };
              int ca = category(a);
              int cb = category(b);
              if (ca != cb)
                return ca < cb;
              return a.get_path().filename() < b.get_path().filename();
            });


  update_selected_index();
}

// Indice nella lista visualizzata (con eventuali buchi)
FileEntry &Panel::get_file_at(int i) {
  return const_cast<FileEntry &>(get_file_list().at(i));
}

// Può restituire un placeholder: il chiamante deve controllare is_placeholder()
const FileEntry &Panel::get_current_file() const {
  return get_file_list().at(selected_index);
}

FileEntry &Panel::get_current_file() {
  return const_cast<FileEntry &>(
      static_cast<const Panel *>(this)->get_current_file());
}

void Panel::show_hidden_files(bool h) { show_hidden = h; }

bool Panel::is_showing_hidden() const { return show_hidden; }

int Panel::get_selected_index() const { return selected_index; }

void Panel::set_selected_index(int i) { selected_index = i; }

void Panel::update_selected_index() {
  if (selected_index >= static_cast<int>(get_file_list().size()))
    selected_index = static_cast<int>(get_file_list().size()) - 1;
  if (selected_index == -1)
    selected_index = 0;
}

// Restituisce il nome del file corrente, o "" se è un placeholder
std::string Panel::get_current_file_name() const {
  if (get_file_list().empty())
    return "";
  const FileEntry &fe = get_file_list().at(selected_index);
  if (fe.is_placeholder())
    return "";
  return fe.get_name();
}

// Restituisce il path del file corrente — chiamante deve assicurarsi che non
// sia un placeholder
std::filesystem::path Panel::get_current_file_fullpath() const {
  const FileEntry &fe = get_file_list().at(selected_index);
  assert(!fe.is_placeholder());
  return fe.get_path();
}

void Panel::move_up() {
  if (selected_index > 0)
    selected_index--;
}

void Panel::move_down() {
  if (selected_index < (int)get_file_list().size() - 1)
    selected_index++;
}

const std::filesystem::path &Panel::get_current_path() const {
  return current_path;
}

const std::vector<FileEntry> &Panel::get_raw_file_list() const {
  return raw_file_list;
}

const FileEntry &Panel::get_file(int i) const {
  return get_file_list().at(i);
}

Panel* Panel::get_aligned_panel() {
  return sync_partner;
}

bool Panel::go_up() {
  if (current_path != current_path.parent_path()) {
    path_history.push(current_path);
    current_path = current_path.parent_path();
    change_dir(current_path);
    return true;
  }
  return false;
}

bool Panel::go_back() {
  if (path_history.size() > 0) {
    current_path = path_history.top();
    path_history.pop();
    change_dir(current_path);
    return true;
  }
  return false;
}

bool Panel::is_active() const { return active; }

void Panel::set_active(bool a) { active = a; }

// Cerca sempre nei file reali
int Panel::contains(const std::string &name) const {
  for (int i = 0; i < static_cast<int>(raw_file_list.size()); i++) {
    if (raw_file_list.at(i).get_name() == name)
      return i;
  }
  return -1;
}

void Panel::tag_current_file(bool t) {
  // Il chiamante deve assicurarsi che il file corrente non sia un placeholder
  FileEntry &fe = get_current_file();
  if (fe.is_placeholder()) return;
  fe.tag(t);

  if (t) {
    tagged_files.push_back(fe.get_path());
  } else {
    for (auto it = tagged_files.begin(); it != tagged_files.end(); it++) {
      if (*it == fe.get_path()) {
        it = tagged_files.erase(it);
        return;
      }
    }
  }
}

void Panel::toggle_tag_current_file() {
  // Guarda è qui: non chiamare se il cursore è su un placeholder
  if (get_current_file().is_placeholder())
    return;
  tag_current_file(!get_current_file().is_tagged());
}

const std::vector<std::filesystem::path> &Panel::get_tagged_files() const {
  return tagged_files;
}

std::vector<std::filesystem::path> Panel::get_files_to_operate() const {
  if (!tagged_files.empty())
    return tagged_files;
  if (get_file_list().empty())
    return {};
  // Se il cursore è su un buco, niente da operare
  const FileEntry &fe = get_current_file();
  if (fe.is_placeholder())
    return {};
  return {fe.get_path()};
}

bool Panel::has_sync_partner() const { return sync_partner != nullptr; } 
