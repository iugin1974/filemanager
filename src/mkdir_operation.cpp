#include "mkdir_operation.h"
#include <filesystem>

void MkdirOperation::execute(const std::filesystem::path& path) {
    std::filesystem::create_directory(path);
}

void MkdirOperation::execute(const std::filesystem::path&,
                               const std::filesystem::path&) {
    // non usato per mkdir
}
