#include "mkdir_operation.h"
#include <filesystem>

void MkdirOperation::execute(const std::filesystem::path& path) {
    std::filesystem::create_directory(path);
}

void MkdirOperation::execute(const std::filesystem::path& path,
                               const std::filesystem::path& path2) {
    // non usato per mkdir
}
