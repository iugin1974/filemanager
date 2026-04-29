#include "delete_operation.h"
#include <filesystem>
#include "popup.h"

void DeleteOperation::execute(const std::filesystem::path& path) {
    bool remove_anyway = show_popup(path);
    
}

void DeleteOperation::execute(const std::filesystem::path& path,
                               const std::filesystem::path& path2) {
}

bool DeleteOperation::show_popup(const std::filesystem::path& path) {
    int choice = Popup::show(
        {
            "Delete file?",
            "",
            path.filename().string(),
        },
        {"[Y]es", "[N]o"}
    );
    return choice == 0;
}
