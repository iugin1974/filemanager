#include <vector>
#include "history.h"
#include "history_element.h"
#include <fstream>

void History::put_element(HistoryElement e) {
    delete_history_right();
    history.push_back(e);
    index = static_cast<int>(history.size()) - 1;
}

HistoryElement& History::move_left() {
    if (index > 0)
        index--;

    return history.at(index);
}

HistoryElement& History::move_right() {
    if (index < static_cast<int>(history.size()) - 1) index++;
    return history.at(index);
}

void History::delete_history_right() {
    if (history.empty()) return;
    auto it = history.begin() + index + 1;
    history.erase(it, history.end());
}

bool History::empty() const {
    return history.empty();
}

bool History::is_at_end() const {
    return !history.empty() &&
           index == static_cast<int>(history.size()) - 1;
}

bool History::can_go_left() const {
    return index > 0;
}

bool History::can_go_right() const {
    return !history.empty() &&
           index < static_cast<int>(history.size()) - 1;
}

HistoryElement& History::last() {
    return history.back();
}

void History::set_current_file_index(int i) {
        history.at(index).set_selected_index(i);
}

void History::print(const std::string msg, int logId) const {
    std::string logPath;

    switch (logId) {
        case 1:
            logPath = "/tmp/log1";
            break;
        case 2:
            logPath = "/tmp/log2";
            break;
        default:
            logPath = "/tmp/log"; // fallback opzionale
            break;
    }

    std::ofstream log(logPath, std::ios::app);

    log << msg << std::endl;

    for (int i = 0; i < static_cast<int>(history.size()); i++) {
        log << (i == index ? ">> " : "   ");
        log << history[i].get_path() << std::endl;
    }

    log << "---" << std::endl;
}
