#include "mkdir_operation.h"
#include "file_entry.h"

void MkdirOperation::execute(const FileEntry& entry) {
    std::filesystem::create_directory(entry.get_path());
}

void MkdirOperation::execute(const FileEntry&, const FileEntry&) {
    // non usato per mkdir
}
