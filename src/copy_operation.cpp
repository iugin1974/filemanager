#include "copy_operation.h"
#include "file_entry.h"
#include <string>
#include "popup.h"

void CopyOperation::execute(const FileEntry&) {
}

void CopyOperation::execute(const FileEntry& source, const FileEntry& dest) {
    std::filesystem::copy(
        source.get_path(),
        dest.get_path(),
        std::filesystem::copy_options::overwrite_existing |
        std::filesystem::copy_options::recursive
    );
}
