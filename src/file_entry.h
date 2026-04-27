#pragma once

#include <ncurses.h>
#include <filesystem>
#include <string.h>

class FileEntry {
  public:
    FileEntry(const std::filesystem::directory_entry& entry);
    void print(WINDOW* win, int row, bool selected) const;
    bool is_directory() const;
    std::filesystem::path get_path() const;
    const std::string get_name() const; 
  private:
    std::filesystem::directory_entry entry;
};
