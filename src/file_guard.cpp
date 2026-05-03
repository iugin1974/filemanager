#include "file_guard.h"
#include "popup.h"
#include <string>
#include <filesystem>


bool FileGuard::confirm_overwrite(const std::filesystem::path& source,
                                const std::filesystem::path& dest) {
    
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


bool FileGuard::confirm_overwrite(const std::filesystem::path& source) {
    
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


bool FileGuard::confirm_delete(const std::filesystem::path& path) {
    int choice = Popup::show(
        {
            "Confirm delete?",
            "",
            "" + file_info(path),
        },
        {"[Y]es", "[N]o"}
    );
    return choice == 0;
}

std::string FileGuard::file_info(const std::filesystem::path& p) {
    if (std::filesystem::is_directory(p)) {
        auto count = std::distance(
    std::filesystem::recursive_directory_iterator(p),
    std::filesystem::recursive_directory_iterator{}
);
         return p.filename().string()+ "  (" + std::to_string(count)+ " files)" ;
    }
    auto size = std::filesystem::file_size(p);
    auto ftime = std::filesystem::last_write_time(p);
    
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - std::filesystem::file_time_type::clock::now()
        + std::chrono::system_clock::now()
    );
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
    
    char buf[20];
    std::strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M", std::localtime(&tt));
    
    return p.filename().string() + "  " + std::to_string(size) + " bytes  " + buf;
}

