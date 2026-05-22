#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include "panel.h"
class FileComparator {
    
public:
    void start(Panel& a, Panel& b);
    void stop();
    bool is_running() const;
    ~FileComparator() { stop(); }
    
private:
    Panel* panel_a;
    Panel* panel_b;
    std::thread thread;
    std::atomic<bool> running = false;
    void compare_files_at(const int i);
    void compare_all();
    void compare_files(FileEntry& a, FileEntry& b);
    uint64_t partial_hash(const std::filesystem::path& path, size_t bytes);
};
