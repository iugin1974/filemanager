#include "status_bar.h"
#include <ncurses.h>
#include <string>
#include <chrono>
#include <ctime>
#include <filesystem>

StatusBar::StatusBar(int y_pos, int x_pos, int width) {
  win = newwin(1, width, y_pos, x_pos);
  wattron(win, A_REVERSE | A_BOLD);
}

void StatusBar::print_message(const std::filesystem::path& current_path) {
        werase(win);
    wattron(win, A_REVERSE);
    whline(win, ' ', getmaxx(win));
    mvwprintw(win, 0, 0, "%s", current_path.string().c_str());
    wattroff(win, A_REVERSE);
    wrefresh(win);
}

void StatusBar::print_message(const std::filesystem::path& current_path, const FileEntry& entry) {
    werase(win);
    wattron(win, A_REVERSE);
    whline(win, ' ', getmaxx(win));

    // Path a sinistra
    std::string left = current_path.string();

    // Dimensione in formato leggibile
    std::string size_str;
    std::uintmax_t size = 0;
    if (!entry.is_directory()) {
        
        try {
        size = std::filesystem::file_size(entry.get_path()); // questo perché le directory su cui non ho diritti, non possono venire lette.
        if (size < 1024)
            size_str = std::to_string(size) + " B";
        else if (size < 1024 * 1024)
            size_str = std::to_string(size / 1024) + " KB";
        else
            size_str = std::to_string(size / (1024 * 1024)) + " MB";
        } catch (const std::filesystem::filesystem_error& e) {
         size = 0;   
        }
    } else {
        size_str = "DIR";
    }

    // Data di modifica
    auto ftime = std::filesystem::last_write_time(entry.get_path());
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
    );
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
    std::tm* tm = std::localtime(&tt);
    char date_buf[20];
    std::strftime(date_buf, sizeof(date_buf), "%d.%m.%Y", tm);

    // Stringa destra
    std::string right = size_str + " - " + date_buf;

    // Stampa
    int width = getmaxx(win);
    int right_col = width - right.size() - 1;
    mvwprintw(win, 0, 0, "%s", left.c_str());
    mvwprintw(win, 0, right_col, "%s", right.c_str());

    wattroff(win, A_REVERSE);
    wrefresh(win);
}

void StatusBar::clear() {
  werase(win);
}
