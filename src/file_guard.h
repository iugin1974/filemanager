#pragma once

#include "file_entry.h"

class FileGuard {
public:
  static bool confirm_overwrite(const FileEntry &source, const FileEntry &dest);
  static bool confirm_overwrite(const FileEntry &source);
  static bool confirm_delete(const FileEntry &entry);
  static bool confirm_delete(const FileEntry &entry1, const FileEntry &entry2);

private:
  static std::string file_info(const FileEntry &entry);
};
