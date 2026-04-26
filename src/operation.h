#pragma once

#include <filesystem>

class Operation {
public:
    virtual void execute(const std::filesystem::path& path) = 0;
    virtual void execute(const std::filesystem::path& path1,
                          const std::filesystem::path& path2) = 0;
    virtual ~Operation() = default;
};
