#pragma once
#include "file_entry.h"

class Operation {
public:
    virtual void execute(const FileEntry& entry) = 0;
    virtual void execute(const FileEntry& source, const FileEntry& dest) = 0;
    virtual ~Operation() = default;

protected:
    bool show_popup(const FileEntry& source, const FileEntry& dest);
    std::string file_info(const FileEntry& entry);
};
