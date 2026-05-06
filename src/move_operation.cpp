#include "move_operation.h"
#include "file_entry.h"

void MoveOperation::execute(const FileEntry&) {
}

void MoveOperation::execute(const FileEntry& source, const FileEntry& dest) {
    std::filesystem::copy(
        source.get_path(),
        dest.get_path(),
        std::filesystem::copy_options::overwrite_existing |
        std::filesystem::copy_options::recursive
    );
    std::filesystem::remove_all(source.get_path());
}
