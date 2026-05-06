#pragma once
#include "file_entry.h"
#include "operation.h"

class DeleteOperation : public Operation {
public:
    void execute(const FileEntry& entry) override;
    void execute(const FileEntry& source, const FileEntry& dest) override;
};
