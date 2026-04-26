#include "move_operation.h"
#include <filesystem>

void MoveOperation::execute(const std::filesystem::path& path) {
}

void MoveOperation::execute(const std::filesystem::path& path,
                               const std::filesystem::path& path2) {
    std::filesystem::rename(path, path2);
}
