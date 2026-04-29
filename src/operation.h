#pragma once

#include <filesystem>

class Operation {
public:
    virtual void execute(const std::filesystem::path& path) = 0;
    virtual void execute(const std::filesystem::path& path1,
                          const std::filesystem::path& path2) = 0;
    virtual ~Operation() = default;
    
protected:
        bool show_popup(const std::filesystem::path& source, const std::filesystem::path& dest);
    std::string file_info(const std::filesystem::path& p);
};
