#include "controller.h"
#include "view.h"
#include "panel.h"
#include "command_bar.h"
#include "command.h"
#include "popup.h"
#include "delete_operation.h"
#include "copy_operation.h"
#include "mkdir_operation.h"
#include "move_operation.h"
#include "touch_operation.h"
#include "operation.h"
#include "file_guard.h"
#include <unistd.h>

#define ctrl(x) ((x) & 0x1f) // definisce CTRL+H

// ---------------------------------------------------------------------------
// Costruttore
// ---------------------------------------------------------------------------

Controller::Controller(View& view) : view(view) {
    reload_panels();
    panels[0].set_active(true);
    view.init_panels(&panels[0], &panels[1]);
    view.draw_panels();
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

bool Controller::handle_key(int ch) {
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
            
        case KEY_RIGHT:
            go_back();
            break;
            
        case KEY_ENTER:
        case 10:
        case 13:
            for_active_panels([this](Panel& p, int i) {
                enter_pressed(p.get_selected_index(), i);
            });
            break;
        case ctrl('h'): {
            for (int i = 0; i < 2; i++) {
                panels.at(i).show_hidden_files(!panels.at(i).is_showing_hidden());
                panels.at(i).reload();
            }
            break;    
        }
        case ctrl('t'): toggle_tag_file();
            break;
        case 9: // TAB
            change_active_panel();
            break;
            
        case ':': {
            std::string cmd = get_command();
            evaluate_command(cmd);
            break;
        }
        case '$': {
         reload_panels();   
        }
        default:
            jump_to_file(ch);
            break;
    }
    
    view.draw_panels();
    return exit;
}

void Controller::exit_status() {
    exit = true;
}
// ---------------------------------------------------------------------------
// Azioni
// ---------------------------------------------------------------------------

void Controller::reload_panels() {
    panels[0].reload();
    panels[1].reload();
}

void Controller::enter_pressed(int selected_line, int panel_index) {
    Panel& panel = panels[panel_index];
    if (panel.get_files().size() == 0) return;
    FileEntry entry = panel.get_file(selected_line);
    
    if (entry.is_directory()) {
        panel.change_dir(entry.get_path());
        view.draw_panels();
    } else {
        pid_t new_process = fork();
        if (new_process == 0) {
        execlp("xdg-open", "xdg-open", entry.get_path().string().c_str(), nullptr);
        _exit(1);
        } else if (new_process < 0) {
         Popup::show({"Error: cannot open file"}, {"[OK]"});   
        }
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

void Controller::go_back() {
 bool moved = false;
 
    for_active_panels([&moved](Panel& p, int) {
        bool ok = p.go_back();
        moved = moved || ok;
    });
    
    if (moved)
        view.draw_panels();
}

void Controller::sync_move(bool up) {
    int other = (get_active_panel() == 0) ? 1 : 0;
    up ? panels[get_active_panel()].move_up() : panels[get_active_panel()].move_down();
    std::string name = panels[get_active_panel()].get_current_file_name();
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

void Controller::jump_to_file(char ch) {
    Panel& p = panels.at(get_active_panel());
    for (int i = p.get_selected_index() + 1; i < p.get_files().size(); i++) {
        std::string name = p.get_file_at(i).get_name();
        if (name[0] == ch) {
            p.set_selected_index(i);
            p.reload();
            view.set_offset(p, i);
            view.draw_panels();
            return;
        }
    }

    for (int i = 0; i < p.get_selected_index(); i++) {
        std::string name = p.get_file_at(i).get_name();
        if (name[0] == ch) {
            p.set_selected_index(i);
            p.reload();
            view.draw_panels();
            return;
        }
    }
}

void Controller::toggle_tag_file() {
 Panel& p = panels.at(get_active_panel());
 p.toggle_tag_current_file();
 p.move_down();
 view.draw_panels();
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
    for (int i = 0; i < 2; i++) panels[i].reload();
    view.draw_panels();
    
}

void Controller::set_sync(bool sync) {
    if (!sync) {
        sync_mode = false;
        return;
    }
    // sync on può solo essere attivato se i due cursori sono su un file con lo stesso nome.
    // se non esiste, non si attiva
    std::string n1 = panels[0].get_current_file_name();
    int index = panels[1].contains(n1);
    if (index == -1) {
        sync_mode = false;
        return;
    }
    
    panels[1].set_selected_index(index);
    if (sync) sync_mode = true;
}

void Controller::delete_file(bool silent) {
      Panel& p = panels[get_active_panel()];
    if (p.get_files().size() == 0) return;

    const std::vector<std::filesystem::path>& tagged = p.get_tagged_files();
    
    std::vector<std::filesystem::path> to_delete;
    if (tagged.empty())
        to_delete.push_back(p.get_current_file_fullpath());
    else
        to_delete = tagged;

    DeleteOperation d;
    for (const auto& f : to_delete) {
        bool ok = true;
        if (!silent) ok = FileGuard::confirm_delete(f);
        if (ok) d.execute(f);
    }
}

void Controller::copy_file() {
    Panel& p1 = panels[get_active_panel()];
    if (p1.get_files().size() == 0) return;
    Panel& p2 = panels[1 - get_active_panel()];
    
    std::filesystem::path source = p1.get_current_file_fullpath();
    
    std::filesystem::path destination =
    p2.get_current_path() / p1.get_current_file_name();
    
    bool ok = true;
    if (std::filesystem::exists(destination))
    ok = FileGuard::confirm_overwrite(source, destination);
    if (ok) {
    CopyOperation c;
    c.execute(source, destination);
    }
}

void Controller::mkdir(const std::string& name) {
    Panel& p = panels[get_active_panel()];
    std::filesystem::path path = p.get_current_path();
    MkdirOperation m;
    m.execute(path / name);
}

void Controller::move_file(const std::string& name) {
    Panel& p1 = panels[get_active_panel()];
    if (p1.get_files().size() == 0) return;
    
    std::filesystem::path source = p1.get_current_file_fullpath();
    std::filesystem::path destination;
    
    destination = p1.get_current_path() / name;
    MoveOperation m;
    m.execute(source, destination);
}

void Controller::move_file() {
    // non posso usare std::filesystem::renamve (vedi MoveOperation)
    // perché rename non funziona su partizioni diverse
    // rename: Invalid cross-device link
    // quindi lo copio prima e poi lo cancello
    copy_file();
    delete_file(true);
}

void Controller::touch(const std::string& name) {
    Panel& p = panels[get_active_panel()];
    std::filesystem::path path = p.get_current_path();
    TouchOperation m;
    m.execute(path / name);
}
