#include "file_guard.h"
#include "popup.h"
#include "file_entry.h"
#include <string>
#include <filesystem>

bool FileGuard::confirm_delete(const FileEntry& entry) {
    int choice = Popup::show(
        {
            "Confirm delete?",
            "",
            file_info(entry),
        },
        {"[Y]es", "[N]o"}
    );
    return choice == 0;
}

bool FileGuard::confirm_delete(const FileEntry& entry1, const FileEntry& entry2) {
    int choice = Popup::show(
        {
            "Confirm delete from both panels?",
            "",
            file_info(entry1),
            file_info(entry2),
        },
        {"[Y]es", "[N]o"}
    );
    return choice == 0;
}

bool FileGuard::confirm_overwrite(const FileEntry& source, const FileEntry& dest) {
    int choice = Popup::show(
        {
            "File already exists!",
            "",
            "Source: " + file_info(source),
            "Dest:   " + file_info(dest),
        },
        {"[Y]es", "[N]o"}
    );
    return choice == 0;
}

bool FileGuard::confirm_overwrite(const FileEntry& source) {
    int choice = Popup::show(
        {
            "File already exists!",
            "",
            "Source: " + file_info(source),
        },
        {"[Y]es", "[N]o"}
    );
    return choice == 0;
}

std::string FileGuard::file_info(const FileEntry& entry) {
    try {
        if (entry.is_directory()) {
            auto count = std::distance(
                std::filesystem::recursive_directory_iterator(entry.get_path()),
                std::filesystem::recursive_directory_iterator{}
            );
            return entry.get_name() + "  (" + std::to_string(count) + " files)";
        }
        auto size = std::filesystem::file_size(entry.get_path());
        auto ftime = entry.get_last_write_time();
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - std::filesystem::file_time_type::clock::now()
            + std::chrono::system_clock::now()
        );
        std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M", std::localtime(&tt));
        return entry.get_name() + "  " + std::to_string(size) + " bytes  " + buf;
    } catch (const std::filesystem::filesystem_error&) {
        return entry.get_name() + "  (permission denied)";
    }
}
