#pragma once
#include <ncurses.h>
#include <filesystem>
#include "file_entry.h"
class StatusBar {
  public:
    StatusBar() : win(nullptr) {}
    StatusBar(int y_pos, int x_pos, int width);
    void print_message(const std::filesystem::path& path, const FileEntry&, const size_t file_number, int index);
    void print_message(const std::filesystem::path& path);
    void clear();

  private:
    WINDOW* win;

};
