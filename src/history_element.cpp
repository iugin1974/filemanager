#include <filesystem>
#include "history_element.h"


HistoryElement::HistoryElement(const std::filesystem::path& p) {
 path = p;
}

const std::filesystem::path& HistoryElement::get_path() const {
    return path;
}

int HistoryElement::get_selected_index() const {
    return selected_index;
}

void HistoryElement::set_selected_index(int i) {
    selected_index = i;
}
