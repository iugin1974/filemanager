#include "copy_operation.h"
#include <filesystem>
#include <string>
#include "popup.h"

void CopyOperation::execute(const std::filesystem::path& path) {
}
// non usato per copy

void CopyOperation::execute(const std::filesystem::path& path,
                               const std::filesystem::path& path2) {
std::filesystem::copy(path, path2, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
                                   
}
