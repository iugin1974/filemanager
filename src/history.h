#pragma once

#include <vector>
#include "history_element.h"

/* Una classe per la mamorizzazione della navigazione nei pannelli.
 * La classe memorizza una serie di HistoryElement, contenenti un link
 * alla directory appena lasciata e all'indice dell'ultima directory selezionata.
 * */
class History {
public:
void put_element(HistoryElement e);
/* Restituisce l'HistoryElement a cui si punta e si sposta verso l'inizio. */
HistoryElement& move_left();
HistoryElement& move_right();

bool empty() const;
bool is_at_end() const;
bool can_go_left() const;
bool can_go_right() const;
void print(std::string msg) const;
HistoryElement& last();
void set_current_file_index(int i);

private:
std::vector<HistoryElement> history;
int index = -1;
void delete_history_right();
};
