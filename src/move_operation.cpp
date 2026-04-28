#include "move_operation.h"
#include <filesystem>
#include "popup.h"

void MoveOperation::execute(const std::filesystem::path& path) {
}

void MoveOperation::execute(const std::filesystem::path& path,
                               const std::filesystem::path& path2) {
    bool move_anyway = true;
    if (std::filesystem::exists(path2))
        move_anyway = show_popup();

    if (move_anyway) {
        std::filesystem::copy(path, path2, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
        std::filesystem::remove_all(path);
    }
}

bool MoveOperation::show_popup() {
    int choice = Popup::show({"The file already exists", "Move anyway?"},
                             {"[Y]es", "[N]o"});
    return choice == 0;
}
