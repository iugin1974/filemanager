#include <ncurses.h>
#include <filesystem>
#include "file_entry.h"

FileEntry::FileEntry(const std::filesystem::directory_entry& entry) {
  name = entry.path().filename().string();
  path = entry.path();
  is_dir = entry.is_directory();
}

void FileEntry::print(WINDOW* win, int row, bool selected) const {
  if (selected) wattron(win, A_REVERSE);
  if (is_dir) {
    wattron(win, COLOR_PAIR(1));
    wattron(win, A_BOLD);
  }
  mvwprintw(win, row, 1, "%s", name.c_str());
  wattroff(win, A_REVERSE | COLOR_PAIR(1) | A_BOLD);
}

bool FileEntry::is_directory() {
  return is_dir;
}

const std::string FileEntry::get_name() const {
  return name;
}

std::filesystem::path FileEntry::get_path() const {
  return path;
}

const std::filesystem::path FileEntry::get_full_path() const {
 return path / name; 
}
