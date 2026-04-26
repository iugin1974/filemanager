#include "copy_operation.h"
#include <filesystem>

void CopyOperation::execute(const std::filesystem::path& path) {
}
// non usato per copy
void CopyOperation::execute(const std::filesystem::path& path,
                               const std::filesystem::path& path2) {
    std::filesystem::copy(path, path2);
}
