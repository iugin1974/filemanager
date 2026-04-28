#pragma once

#include "operation.h"
#include <filesystem>
#include <string>

class CopyOperation : public Operation {
public:
    void execute(const std::filesystem::path& path) override;
    void execute(const std::filesystem::path& path1,
                 const std::filesystem::path& path2) override;
                 
private:
bool show_popup(const std::filesystem::path& source,
                                const std::filesystem::path& dest);
std::string file_info(const std::filesystem::path& p);
};
