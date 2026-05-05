#pragma once

#include <filesystem>

class FileGuard {
public:
static bool confirm_overwrite(const std::filesystem::path& source,
                                const std::filesystem::path& dest);
static bool confirm_overwrite(const std::filesystem::path& source);
static bool confirm_delete(const std::filesystem::path& path, const std::filesystem::path& path2);
static bool confirm_delete(const std::filesystem::path& path);

private:

static std::string file_info(const std::filesystem::path& p);
};
