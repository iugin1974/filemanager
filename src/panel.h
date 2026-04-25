#pragma once
#include <filesystem>
#include <vector>
#include "file_entry.h"

class Panel {
  public:
    Panel();
    void change_dir(const std::filesystem::path& path);
    void reload();
    const std::filesystem::path& get_current_path() const;
    const std::vector<FileEntry>& get_files() const;
    int get_selected_index() const;
    void move_up();
    void move_down();
    const FileEntry& get_file(int) const;
    bool go_up();
	bool is_active();
	void set_active(bool);
  private:
    std::filesystem::path current_path;
    std::vector<FileEntry> file_list;
    int selected_index = 0;
    bool active = false;
};
