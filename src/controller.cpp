#include "controller.h"
#include "command.h"
#include "command_bar.h"
#include "copy_operation.h"
#include "delete_operation.h"
#include "file_guard.h"
#include "mkdir_operation.h"
#include "move_operation.h"
#include "operation.h"
#include "panel.h"
#include "popup.h"
#include "touch_operation.h"
#include "view.h"
#include <filesystem>
#include <unistd.h>

#define ctrl(x) ((x)&0x1f) // definisce CTRL+H


#include <fstream>
void Controller::test()
{
    // Crea le cartelle
    std::filesystem::create_directories("/tmp/A");
    std::filesystem::create_directories("/tmp/B");

    // File in A
    for (const auto& name : {"a.txt", "b.txt", "c.txt", "d.txt"})
    {
        std::ofstream("/tmp/A/" + std::string(name));
    }

    // File in B
    for (const auto& name : {"a.txt", "b.txt", "d.txt"})
    {
        std::ofstream("/tmp/B/" + std::string(name));
    }

    // Cambia directory nei due pannelli
    panels[0].change_dir("/tmp/A");
    panels[1].change_dir("/tmp/B");
    sync_mode = true;
    align_panels();
    
}

// ---------------------------------------------------------------------------
// Costruttore
// ---------------------------------------------------------------------------

Controller::Controller(View &view) : view(view) {
  reload_panels();
  panels[0].set_active(true);
  view.init_panels(&panels[0], &panels[1]);
  test();
  view.draw_panels();
}

// ---------------------------------------------------------------------------
// Helpers privati
// ---------------------------------------------------------------------------

int Controller::get_active_panel_index() const {
  return panels[0].is_active() ? 0 : 1;
}

int Controller::get_active_panel_index() {
  return panels[0].is_active() ? 0 : 1;
}

int Controller::get_inactive_panel_index() const {
  return panels[0].is_active() ? 0 : 1;
}

int Controller::get_inactive_panel_index() {
  return panels[0].is_active() ? 0 : 1;
}

Panel &Controller::get_active_panel() {
  return panels[0].is_active() ? panels[0] : panels[1];
}

const Panel &Controller::get_active_panel() const {
  return panels[0].is_active() ? panels[0] : panels[1];
}

Panel &Controller::get_inactive_panel() {
  return panels[0].is_active() ? panels[1] : panels[0];
}

const Panel &Controller::get_inactive_panel() const {
  return panels[0].is_active() ? panels[1] : panels[0];
}

void Controller::change_active_panel() {
  int active = get_active_panel_index();
  panels[active].set_active(false);
  panels[1 - active].set_active(true);
}

// Esegue fn su entrambi i panel (sync_mode) o solo su quello attivo.
// fn riceve (Panel&, int panel_index).
template <typename Fn> void Controller::for_active_panels(Fn fn) {
  if (sync_mode) {
    for (int i = 0; i < 2; ++i)
      fn(panels[i], i);
  } else {
    int i = get_active_panel_index();
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
    enter_pressed();
    break;
  case ctrl('h'): {
    for (int i = 0; i < 2; i++) {
      panels.at(i).show_hidden_files(!panels.at(i).is_showing_hidden());
      panels.at(i).reload();
    }
    break;
  }
  case ctrl('t'):
    toggle_tag_file();
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
    break;
  }
  default:
    jump_to_file(ch);
    break;
  }

  view.draw_panels();
  return exit;
}

void Controller::exit_status() { exit = true; }
// ---------------------------------------------------------------------------
// Azioni
// ---------------------------------------------------------------------------

void Controller::reload_panels() {
  panels[0].reload();
  panels[1].reload();
}

void Controller::enter_pressed() {  
  Panel &active_panel = get_active_panel();
  Panel &other_panel = get_inactive_panel();
  // il panello attivo non ha files
  if (active_panel.get_file_list().size() == 0)
    return;
  // il pannello attivo ha files
  // entry è il file/directory su cui si è cliccato
  FileEntry entry = active_panel.get_file(active_panel.get_selected_index());
  // placeholder: esce
  if (entry.is_placeholder())
    return;
  // directory: viene aperta
  if (entry.is_directory()) {
    active_panel.change_dir(entry.get_path());
    if (sync_mode) {
      other_panel.change_dir(other_panel.get_current_path() / entry.get_name());
      align_panels();
    }
    view.draw_panels();
    return;
  }
  // apre il file selezionato sul pannello attivo
  pid_t new_process = fork();
  if (new_process == 0) {
    execlp("xdg-open", "xdg-open", entry.get_path().string().c_str(), nullptr);
    _exit(1);
  } else if (new_process < 0) {
    Popup::show({"Error: cannot open file"}, {"[OK]"});
  }
}

void Controller::go_up() {
  bool moved = false;

  for_active_panels([&moved](Panel &p, int) {
    bool ok = p.go_up();
    moved = moved || ok;
  });

  if (moved) {
    align_panels();
    view.draw_panels();
  }
}

void Controller::go_back() {
  bool moved = false;

  for_active_panels([&moved](Panel &p, int) {
    bool ok = p.go_back();
    moved = moved || ok;
  });

  if (moved)
    view.draw_panels();
}

void Controller::sync_move(bool sync) {
  int other = (get_active_panel_index() == 0) ? 1 : 0;
  sync ? panels[get_active_panel_index()].move_up()
       : panels[get_active_panel_index()].move_down();
  std::string name = panels[get_active_panel_index()].get_current_file_name();
  int index = panels[other].contains(name);
  if (index != -1)
    panels[other].set_selected_index(index);
}

void Controller::move_up() {
    for_active_panels([](Panel &p, int) { p.move_up(); });
}

void Controller::move_down() {
    for_active_panels([](Panel &p, int) { p.move_down(); });
}

void Controller::jump_to_file(char ch) {
  // auto oggetto = [capture list] (parametri) -> tipo_ritorno { corpo };
  auto jump = [&](Panel &p) -> void {
    for (int i = p.get_selected_index() + 1;
         i < static_cast<int>(p.get_raw_file_list().size()); i++) {
      std::string name = p.get_file_at(i).get_name();
      if (name[0] == ch) {
        p.set_selected_index(i);
        view.set_offset(p, i);
        view.draw_panels();
        return;
      }
    }

    for (int i = 0; i < p.get_selected_index(); i++) {
      std::string name = p.get_file_at(i).get_name();
      if (name[0] == ch) {
        p.set_selected_index(i);
        view.draw_panels();
        return;
      }
    }
  }; // end lambda

  jump(get_active_panel());
  if (sync_mode)
    jump(get_inactive_panel());
}

void Controller::toggle_tag_file() {
  if (sync_mode) {
   CommandBar &c = view.get_command_bar();
    c.print_message("Tagging disabled in sync mode", CommandBar::ERROR);
    return;
  }
  Panel &p = panels.at(get_active_panel_index());
  p.toggle_tag_current_file();
  p.move_down();
  view.draw_panels();
}

void Controller::sync_partner(bool sync) {
  if (sync) {
    panels[0].set_sync_partner(&panels[1]);
    panels[1].set_sync_partner(&panels[0]);
    align_panels();
  } else {
    panels[0].set_sync_partner(nullptr);
    panels[1].set_sync_partner(nullptr);
  }
}
// ---------------------------------------------------------------------------
// Comandi
// ---------------------------------------------------------------------------

std::string Controller::get_command() {
  return view.get_command_bar().get_command();
}

void Controller::evaluate_command(const std::string &cmd) {
  Command command(this);
  command.execute(cmd);
  for (int i = 0; i < 2; i++)
    panels[i].reload();
  if (sync_mode) align_panels();
  view.draw_panels();
}

void Controller::set_sync(bool sync) {
  if (!sync) {
    sync_mode = false;
    sync_partner(false);
    return;
  }
  // sync on può solo essere attivato se i due cursori sono su un file con lo
  // stesso nome. se non esiste, non si attiva
  std::string n1 = panels[0].get_current_file_name();
  if (n1.empty()) {
    sync_mode = false;
    sync_partner(false);
    return;
  }
  int index = panels[1].contains(n1);
  if (index == -1) {
    sync_mode = false;
    sync_partner(false);
    return;
  }

  panels[1].set_selected_index(index);
  sync_mode = true;
  sync_partner(true);
}

void Controller::align_panels() {
  if (!sync_mode) return;
  panels[0].align_with(panels[1].get_raw_file_list());
  panels[1].align_with(panels[0].get_raw_file_list());
} 

void Controller::delete_file(bool silent) {
  Panel &active = get_active_panel();
  Panel &other = get_inactive_panel();
  auto files = active.get_files_to_operate();
  if (files.empty())
    return;
  DeleteOperation d;
  for (const auto &f : files) {
    bool ok = true;
    if (!silent) {
      if (sync_mode) {
        std::filesystem::path other_file = other.get_current_path() / f.filename();
        if (std::filesystem::exists(other_file))
          ok = FileGuard::confirm_delete(f, other_file);
        else
          ok = FileGuard::confirm_delete(f);
      } else {
        ok = FileGuard::confirm_delete(f);
      }
    }
    if (ok) {
      d.execute(f);
      if (sync_mode) {
        std::filesystem::path other_file = other.get_current_path() / f.filename();
        if (std::filesystem::exists(other_file))
          d.execute(other_file);
      }
    }
  }
}

void Controller::copy_file() {
  if (sync_mode) return; // non ha senso in sync mode
  Panel &p1 = get_active_panel();
  Panel &p2 = get_inactive_panel();
  auto files = p1.get_files_to_operate();
  if (files.empty())
    return;
  CopyOperation c;
  for (auto &source : files) {
    std::filesystem::path destination = p2.get_current_path() / source.filename();
    bool ok = true;
    if (std::filesystem::exists(destination))
      ok = FileGuard::confirm_overwrite(source, destination);
    if (ok)
      c.execute(source, destination);
  }
}

void Controller::mkdir(const std::string &name) {
  Panel &active = get_active_panel();
  Panel &other = get_inactive_panel();
  std::filesystem::path active_dir = active.get_current_path() / name;
  if (std::filesystem::exists(active_dir)) {
    view.get_command_bar().print_message("Directory already exists: " + name, CommandBar::ERROR);
    return;
  }
  MkdirOperation m;
  m.execute(active_dir);
  if (sync_mode)
    m.execute(other.get_current_path() / name);
}

void Controller::move_file(const std::string &name) {
  Panel &active = get_active_panel();
  Panel &other = get_inactive_panel();
  if (active.get_file_list().empty())
    return;
  const FileEntry &fe = active.get_current_file();
  if (fe.is_placeholder())
    return;
  std::filesystem::path source = fe.get_path();
  std::filesystem::path destination = active.get_current_path() / name;
  if (std::filesystem::exists(destination)) {
    if (!FileGuard::confirm_overwrite(source, destination))
      return;
  }
  MoveOperation m;
  m.execute(source, destination);
  if (sync_mode) {
    std::filesystem::path other_source = other.get_current_path() / source.filename();
    std::filesystem::path other_destination = other.get_current_path() / name;
    if (std::filesystem::exists(other_source))
      m.execute(other_source, other_destination);
  }
}

void Controller::move_file() {
  if (sync_mode) return;
  // non posso usare std::filesystem::renamve (vedi MoveOperation)
  // perché rename non funziona su partizioni diverse
  // rename: Invalid cross-device link
  // quindi lo copio prima e poi lo cancello
  copy_file();
  delete_file(true);
}

void Controller::touch(const std::string &name) {
  Panel &active = get_active_panel();
  Panel &other = get_inactive_panel();
  std::filesystem::path active_path = active.get_current_path() / name;
  bool ok = true;
  if (std::filesystem::exists(active_path))
    ok = FileGuard::confirm_overwrite(active_path);
  if (ok) {
    TouchOperation t;
    t.execute(active_path);
    if (sync_mode)
      t.execute(other.get_current_path() / name);
  }
}

void Controller::change_dir(const std::string &path) {
  if (path == "..") {
    go_up();
    return;
  }
  std::filesystem::path full_path;
  if (path[0] == '/') {
    full_path = path;
  } else {
    full_path = get_active_panel().get_current_path() / path;
  }
  if (!std::filesystem::exists(full_path)) {
    view.get_command_bar().print_message("Directory not exists", CommandBar::ERROR);
    return;
  }
  Panel &active = get_active_panel();
  Panel &other = get_inactive_panel();
  active.change_dir(full_path);
  if (sync_mode) {
    other.change_dir(other.get_current_path() / std::filesystem::path(path).filename());
    align_panels();
  }
}
