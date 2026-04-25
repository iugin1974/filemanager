#pragma once

#include <ncurses.h>
#include <filesystem>
#include <string.h>

class FileEntry {
  public:
    FileEntry(const std::filesystem::directory_entry& entry);
    void print(WINDOW* win, int row, bool selected) const;
    bool is_directory();
    std::filesystem::path get_path();
    const std::string get_name() const; 

  private:
    std::string name;
    std::filesystem::path path;
    bool is_dir = false;
    bool is_selected = false;
};
