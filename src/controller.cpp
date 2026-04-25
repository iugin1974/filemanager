#include "controller.h"
#include "view.h"
#include "panel.h"
#include "command_bar.h"
#include "command.h"

// ---------------------------------------------------------------------------
// Costruttore
// ---------------------------------------------------------------------------

Controller::Controller(View& view) : view(view) {
    panels[0].reload();
    panels[1].reload();
    panels[0].set_active(true);
    view.init_panels(&panels[0], &panels[1]);
}

// ---------------------------------------------------------------------------
// Helpers privati
// ---------------------------------------------------------------------------

int Controller::get_active_panel() const {
    return panels[0].is_active() ? 0 : 1;
}

void Controller::change_active_panel() {
    int active = get_active_panel();
    panels[active].set_active(false);
    panels[1 - active].set_active(true);
}

// Esegue fn su entrambi i panel (sync_mode) o solo su quello attivo.
// fn riceve (Panel&, int panel_index).
template <typename Fn>
void Controller::for_active_panels(Fn fn) {
    if (sync_mode) {
        for (int i = 0; i < 2; ++i)
            fn(panels[i], i);
    } else {
        int i = get_active_panel();
        fn(panels[i], i);
    }
}

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------

void Controller::handle_key(int ch) {
    switch (ch) {
        case KEY_UP:
            move_up();
            break;

        case KEY_DOWN:
            move_down();
            break;

        case KEY_LEFT:
            go_up();
            break;

        case KEY_ENTER:
        case 10:
        case 13:
            for_active_panels([this](Panel& p, int i) {
                enter_pressed(p.get_selected_index(), i);
            });
            break;

        case 9: // TAB
            change_active_panel();
            break;

        case ':': {
            std::string cmd = get_command();
            evaluate_command(cmd);
            break;
        }

        default:
            break;
    }

    view.draw_panels();
}

// ---------------------------------------------------------------------------
// Azioni
// ---------------------------------------------------------------------------

void Controller::enter_pressed(int selected_line, int panel_index) {
    Panel& panel = panels[panel_index];
    FileEntry entry = panel.get_file(selected_line);

    if (entry.is_directory()) {
        panel.change_dir(entry.get_path());
        view.draw_panels();
    }
}

void Controller::go_up() {
    bool moved = false;

    for_active_panels([&moved](Panel& p, int) {
        bool ok = p.go_up();
        moved = moved || ok;
    });

    if (moved)
        view.draw_panels();
}

void Controller::sync_move(bool up) {
    int other = (get_active_panel() == 0) ? 1 : 0;
    up ? panels[get_active_panel()].move_up() : panels[get_active_panel()].move_down();
    std::string name = panels[get_active_panel()].get_current_file();
    int index = panels[other].contains(name);
    if (index != -1) panels[other].set_selected_index(index);
}

void Controller::move_up() {
    if (!sync_mode) {
        for_active_panels([](Panel& p, int) { p.move_up(); });
        return;
    }
    sync_move(true);
}

void Controller::move_down() {
    if (!sync_mode) {
        for_active_panels([](Panel& p, int) { p.move_down(); });
        return;
    }
    sync_move(false);
}

// ---------------------------------------------------------------------------
// Comandi
// ---------------------------------------------------------------------------

std::string Controller::get_command() {
    return view.get_command_bar(get_active_panel()).get_command();
}

void Controller::evaluate_command(const std::string& cmd) {
  Command command(this);
command.execute(cmd);


  }

void Controller::set_sync(bool sync) {
if (!sync) {
sync_mode = false;
return;
}
// sync on può solo essere attivato se i due cursori sono su un file con lo stesso nome.
// se non esiste, non si attiva
std::string n1 = panels[0].get_current_file();
int index = panels[1].contains(n1);
if (index == -1) {
sync_mode = false;
return;
}

panels[1].set_selected_index(index);
if (sync) sync_mode = true;
}
