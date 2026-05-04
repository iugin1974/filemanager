#pragma once

#include <ncurses.h>
#include <filesystem>
#include <string.h>

class FileEntry {
  public:
    FileEntry(const std::filesystem::directory_entry& entry);
    FileEntry() : placeholder(true) {};
    void print(WINDOW* win, int row, bool selected) const;
    bool is_directory() const;
    std::filesystem::path get_path() const;
    const std::string get_name() const; 
    void tag(bool t);
    void toggle_tag();
    bool is_tagged() const;
  private:
    std::filesystem::directory_entry entry;
    bool tagged = false;
    bool placeholder = false;
};
