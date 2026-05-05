#include <ncurses.h>
#include <filesystem>
#include "file_entry.h"


    
FileEntry::FileEntry(const std::filesystem::directory_entry& entry) {
    this->entry = entry;
    last_write_time = entry.last_write_time();
}

void FileEntry::print(WINDOW* win, int row, bool selected) const {
    if (selected) wattron(win, A_REVERSE);
    if (entry.is_directory()) {
        wattron(win, COLOR_PAIR(1));
        wattron(win, A_BOLD);
    }
    if (sync_status == SyncStatus::NEWER) {
     wattron(win, COLOR_PAIR(2));   
    } else
        if (sync_status == SyncStatus::OLDER) {
            wattron(win, COLOR_PAIR(4));
        } 
    if (tagged)
        mvwprintw(win, row, 0, "*");
    mvwprintw(win, row, 1, "%s", entry.path().filename().string().c_str());
    wattroff(win, A_REVERSE | A_BOLD);
    wattroff(win, A_COLOR);
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

SyncStatus FileEntry::get_sync_status() const {
    return sync_status;
}

void FileEntry::set_sync_status(SyncStatus s) {
 sync_status = s;   
}

const std::filesystem::file_time_type& FileEntry::get_last_write_time() const {
 return last_write_time;   
}
