#include "touch_operation.h"
#include "file_entry.h"
#include <fstream>

void TouchOperation::execute(const FileEntry& entry) {
    std::ofstream file(entry.get_path());
}

void TouchOperation::execute(const FileEntry&, const FileEntry&) {
    // non usato per touch
}
