#include "copy_operation.h"
#include <filesystem>
#include <string>
#include "popup.h"

void CopyOperation::execute(const std::filesystem::path& path) {
}
// non usato per copy

void CopyOperation::execute(const std::filesystem::path& path,
                               const std::filesystem::path& path2) {
    bool copy_anyway = true;
    if (std::filesystem::exists(path2))
        copy_anyway = show_popup(path, path2);
    
    if (copy_anyway)
std::filesystem::copy(path, path2, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
                                   
}

bool CopyOperation::show_popup(const std::filesystem::path& source,
                                const std::filesystem::path& dest) {
    auto info = [](const std::filesystem::path& p) -> std::string {
        auto size = std::filesystem::file_size(p);
        auto time = std::filesystem::last_write_time(p);
        // formatta size
        std::string size_str = std::to_string(size) + " bytes";
        return p.filename().string() + "  " + size_str;
    };
    
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

std::string CopyOperation::file_info(const std::filesystem::path& p) {
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
