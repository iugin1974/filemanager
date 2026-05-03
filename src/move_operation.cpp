#include "move_operation.h"
#include <filesystem>

void MoveOperation::execute(const std::filesystem::path&) {
}

void MoveOperation::execute(const std::filesystem::path& path,
                               const std::filesystem::path& path2) {
        std::filesystem::copy(path, path2, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
        std::filesystem::remove_all(path);
}

