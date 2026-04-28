#include "copy_operation.h"
#include <filesystem>
#include "popup.h"

void CopyOperation::execute(const std::filesystem::path& path) {
}
// non usato per copy

void CopyOperation::execute(const std::filesystem::path& path,
                               const std::filesystem::path& path2) {
    bool copy_anyway = true;
    if (std::filesystem::exists(path2))
        copy_anyway = show_popup();
    
    if (copy_anyway)
std::filesystem::copy(path, path2, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
                                   
}

bool CopyOperation::show_popup() {
int choice = Popup::show({"The file already exists", "Copy anyway?"},
                                 {"[Y]es", "[N]o"});
return choice == 0;
}

