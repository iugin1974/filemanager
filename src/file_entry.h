#pragma once

#include <ncurses.h>
#include <filesystem>
#include <string.h>

// SAME: stesso nome in entrambi i pannelli, contenuto identico (stessa dimensione + hash uguale)
// NONE: non confrontabile (placeholder, directory, ecc.)
enum class SyncStatus { NONE, SAME, NEWER, OLDER, ONCE, DIR };

class FileEntry {
  public:

    // FileEntry deve essere sempre costruita DOPO che il file esiste sul disco.
// Non costruire FileEntry su path inesistenti: last_write_time non sarebbe disponibile.
// Sequenza corretta:
//   1. Crea il file/directory (TouchOperation, MkdirOperation, ecc.)
//   2. Costruisci FileEntry sul path appena creato
    FileEntry(const std::filesystem::directory_entry& entry);
    // FileEntry deve essere sempre costruita DOPO che il file esiste sul disco.
// Non costruire FileEntry su path inesistenti: last_write_time non sarebbe disponibile.
// Sequenza corretta:
//   1. Crea il file/directory (TouchOperation, MkdirOperation, ecc.)
//   2. Costruisci FileEntry sul path appena creato
    explicit FileEntry(const std::filesystem::path& path);
    FileEntry() : placeholder(true) {};
    bool operator==(const FileEntry& other) const;
    void print(WINDOW* win, int row, bool selected, int width, bool active_panel) const;
    bool is_directory() const;
    std::filesystem::path get_path() const;
    const std::string get_name() const;
    void tag(bool t);
    void toggle_tag();
    bool is_tagged() const;
    bool is_placeholder() const;
    const std::filesystem::file_time_type& get_last_write_time() const;
    void set_sync_status(SyncStatus s);
    SyncStatus get_sync_status() const;
    bool exists();
  private:
    std::filesystem::directory_entry entry;
    bool tagged = false;
    bool placeholder = false;
    std::filesystem::file_time_type last_write_time;
    SyncStatus sync_status = SyncStatus::NONE;
};
