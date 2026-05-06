#include "delete_operation.h"
#include "file_entry.h"

void DeleteOperation::execute(const FileEntry& entry) {
    std::filesystem::remove_all(entry.get_path());
}

void DeleteOperation::execute(const FileEntry&, const FileEntry&) {
    // non usato per delete
}
