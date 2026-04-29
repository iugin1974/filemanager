#pragma once

#include "operation.h"
#include <filesystem>

class TouchOperation : public Operation {
public:
    void execute(const std::filesystem::path& path) override;
    void execute(const std::filesystem::path& path1,
                 const std::filesystem::path& path2) override;
                 
};
