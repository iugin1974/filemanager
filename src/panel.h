#pragma once
#include <filesystem>
#include <vector>
#include "file_entry.h"

class Panel {
  public:
    void change_dir(const std::filesystem::path& path);
    void reload();
    const std::filesystem::path& get_current_path() const;
    const std::vector<FileEntry>& get_files() const;
    int get_selected_index() const;
    void move_up();
    void move_down();
    const FileEntry& get_file(int) const;
    bool go_up();

  private:
    std::filesystem::path current_path = std::getenv("HOME");
    std::vector<FileEntry> file_list;
    int selected_index = 0;
};
