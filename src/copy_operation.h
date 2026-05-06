#pragma once

#include "operation.h"
#include "file_entry.h"
#include <string>

class CopyOperation : public Operation {
public:
    void execute(const FileEntry& entry) override;
    void execute(const FileEntry& source, const FileEntry& dest) override;
};
