#pragma once

#include <filesystem>

/*
 * Snapshot di una directory visitata nella navigation history.
 * Memorizza il path della directory e l'elemento selezionato.
 */
class HistoryElement {
public:
    HistoryElement(
        const std::filesystem::path& path
    );

    const std::filesystem::path& get_path() const;
    int get_selected_index() const;
    void set_selected_index(int i);

private:
    std::filesystem::path path;
    int selected_index = 0;
};
