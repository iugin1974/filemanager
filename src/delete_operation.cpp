#include "delete_operation.h"
#include <filesystem>

void DeleteOperation::execute(const std::filesystem::path& path) {
    std::filesystem::remove_all(path);
}

void DeleteOperation::execute(const std::filesystem::path&,
                               const std::filesystem::path&) {
    // non usato per delete
}
