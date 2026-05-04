#include <ncurses.h>
#include <filesystem>
#include "file_entry.h"

FileEntry::FileEntry(const std::filesystem::directory_entry& entry) {
    this->entry = entry;
}

void FileEntry::print(WINDOW* win, int row, bool selected) const {
    if (selected) wattron(win, A_REVERSE);
    if (entry.is_directory()) {
        wattron(win, COLOR_PAIR(1));
        wattron(win, A_BOLD);
    }
    if (tagged)
        mvwprintw(win, row, 0, "*");
    mvwprintw(win, row, 1, "%s", entry.path().filename().string().c_str());
    wattroff(win, A_REVERSE | COLOR_PAIR(1) | A_BOLD);
}

bool FileEntry::is_directory() const {
    return entry.is_directory();
}

const std::string FileEntry::get_name() const {
    return entry.path().filename().string();
}

std::filesystem::path FileEntry::get_path() const {
    return entry.path();
}

void FileEntry::tag(bool t) {
    tagged = t;
}

void FileEntry::toggle_tag() {
    tagged = !tagged;
}

bool FileEntry::is_tagged() const {
    return tagged;
}

bool FileEntry::is_placeholder() const {
 return placeholder;   
}
