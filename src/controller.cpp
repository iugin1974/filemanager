#include "controller.h"
#include "command.h"
#include "command_bar.h"
#include "copy_operation.h"
#include "delete_operation.h"
#include "file_entry.h"
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
void Controller::test() {
  // Crea le cartelle
  std::filesystem::create_directories("/tmp/A");
  std::filesystem::create_directories("/tmp/B");
  
  // File in A
  for (const auto &name : {"0.txt", "a.txt", "b.txt", "c.txt", "d.txt"}) {
    std::ofstream("/tmp/A/" + std::string(name));
  }
  
  // File in B
  for (const auto &name : {"a.txt", "b.txt", "d.txt"}) {
    std::ofstream("/tmp/B/" + std::string(name));
  }
  
  // Cambia directory nei due pannelli
  panels[0].change_dir("/tmp/A");
  panels[1].change_dir("/tmp/B");
  //set_sync(true);
  
  view.draw_panels(sync_mode);
  //align_panels();
  reload_panels();
}

// ---------------------------------------------------------------------------
// Costruttore
// ---------------------------------------------------------------------------
Controller::Controller(View &view)
: view(view),
panels{{1, 2}}
{
  init();
 // test();
}

void Controller::init() {           
  reload_panels();
  panels[0].set_active(true);
  view.init_panels(&panels[0], &panels[1]);
  view.draw_panels(sync_mode);
}

void Controller::on_resize() {
  view.init();                              // ricalcola dimensioni
  view.init_panels(&panels[0], &panels[1]); // ricrea Panel_view
  view.draw_panels(sync_mode);              // ridisegna
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
  return panels[0].is_active() ? 1 : 0;
}

int Controller::get_inactive_panel_index() {
  return panels[0].is_active() ? 1 : 0;
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
    case KEY_NPAGE:
      move(1, view.get_height());
      break;
      
    case KEY_PPAGE:
      move(-1, view.get_height());
      break;
      
    case KEY_UP:
      move(-1, 1);
      break;
      
    case KEY_DOWN:
      move(1, 1);
      break;
      
    case KEY_LEFT:
      go_left();
      break;
      
    case KEY_RIGHT:
      go_right();
      break;
      
    case KEY_HOME:
      go_first();
      break;
      
    case KEY_END:
      go_last();
      break;
      
    case KEY_ENTER:
    case 10:
    case 13:
      enter_pressed();
      break;
    case '-':
      go_up();
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
      if (!cmd.empty())
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
  
  view.draw_panels(sync_mode);
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
  Panel &inactive_panel = get_inactive_panel();
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
      inactive_panel.change_dir(inactive_panel.get_current_path() / entry.get_name());
      align_panels();
    }
    view.draw_panels(sync_mode);
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

void Controller::go_left() {
  bool moved = false;
  
  for_active_panels([&moved](Panel &p, int) {
    bool ok = p.go_left();
    moved = moved || ok;
  });
  
  if (moved) {
    align_panels();
    view.draw_panels(sync_mode);
  }
}

void Controller::go_right() {
  bool moved = false;
  
  for_active_panels([&moved](Panel &p, int) {
    bool ok = p.go_right();
    moved = moved || ok;
  });
  
  if (moved) {
    align_panels();
    view.draw_panels(sync_mode);
  }
}

void Controller::go_up() {
  bool changed = false;
  for_active_panels([&changed](Panel &p, int) {
    bool ok = p.go_up();
    changed = changed || ok;
  });
  if (changed) {
    align_panels();
    view.draw_panels(sync_mode);
  }
}

void Controller::go_first() {
  for_active_panels([](Panel &p, int) {
    p.select_first();
  });
}

void Controller::go_last() {
  for_active_panels([](Panel &p, int) {
    p.select_last();
  });
}

void Controller::move(int direction, int lines) {
  if (direction == -1)
    for_active_panels([lines](Panel &p, int) { p.move_up(lines); });
  else if (direction == 1)
    for_active_panels([lines](Panel &p, int) { p.move_down(lines); });
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
      view.draw_panels(sync_mode);
      return;
    }
         }
         
         for (int i = 0; i < p.get_selected_index(); i++) {
           std::string name = p.get_file_at(i).get_name();
           if (name[0] == ch) {
             p.set_selected_index(i);
             view.draw_panels(sync_mode);
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
  p.move_down(1);
  view.draw_panels(sync_mode);
}

void Controller::sync_partner(bool sync) {
  if (sync) {
    panels[0].set_sync_partner(&panels[1]);
    panels[1].set_sync_partner(&panels[0]);
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
  if (sync_mode) {
    align_panels();
    for (int i = 0; i < 2; i++)
      panels[i].update_selected_index();
  }
  view.draw_panels(sync_mode);
}

void Controller::jump_to_next_different() {
  if (!sync_mode) return;
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  if (active.get_file_list().size() == 0) return;
  if (inactive.get_file_list().size() == 0) return;

  int start = active.get_selected_index();
  int size = active.get_file_list().size();

  // parte dal file successivo, wrappa fino a start
  for (int n = 1; n < size; n++) {
    int i = (start + n) % size;
    auto f = active.get_file_at(i);
    if (f.get_sync_status() != SyncStatus::SAME) {
      active.set_selected_index(i);
      inactive.set_selected_index(i);
      return;
    }
  }
}

void Controller::set_sync(bool sync) {
  if (!sync) {
    sync_mode = false;
    sync_partner(false);
    return;
  }
  sync_mode = true;
  sync_partner(true);
  align_panels();
  sync_index();
  view.draw_panels(sync_mode);
}

void Controller::sync_index() {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel(); 
  inactive.set_selected_index(active.get_selected_index());
}

void Controller::align_panels() {
  if (!sync_mode)
    return;
  panels[0].align_with(panels[1].get_raw_file_list());
  panels[1].align_with(panels[0].get_raw_file_list());
}

void Controller::delete_file(bool silent) {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  auto files = active.get_files_to_operate();
  if (files.empty())
    return;
  DeleteOperation d;
  for (const auto &f : files) {
    FileEntry inactive_file(inactive.get_current_path() / f.get_name());
    bool has_pair = sync_mode && inactive_file.exists();

    // conferma
    if (!silent) {
      bool ok = has_pair ? FileGuard::confirm_delete(f, inactive_file)
                         : FileGuard::confirm_delete(f);
      if (!ok) continue;
    }

    // cancella
    d.execute(f);
    if (has_pair)
      d.execute(inactive_file);
  }
}

void Controller::copy_file() {
  if (sync_mode) {
    sync_file();
    return;
  }
  Panel &p1 = get_active_panel();
  Panel &p2 = get_inactive_panel();
  auto files = p1.get_files_to_operate();
  if (files.empty())
    return;
  CopyOperation c;
  for (auto &source : files) {
    FileEntry destination(p2.get_current_path() / source.get_name());
    bool ok = true;
    if (destination.exists())
      ok = FileGuard::confirm_overwrite(source, destination);
    if (ok)
      c.execute(source, destination);
  }
}

void Controller::sync_file() {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  FileEntry f1 = active.get_current_file();
  FileEntry f2 = inactive.get_current_file();
  CopyOperation c;
  if (f2.is_placeholder()) {
    c.execute(f1, FileEntry(inactive.get_current_path() / f1.get_name())); 
  }
  else if (f1.is_placeholder()) {
    c.execute(f2, FileEntry(active.get_current_path() / f2.get_name())); 
  }
  else if (f1.get_sync_status() == SyncStatus::NEWER && f2.get_sync_status() == SyncStatus::OLDER) {
    c.execute(f1, f2);
  }
  else if (f2.get_sync_status() == SyncStatus::NEWER && f1.get_sync_status() == SyncStatus::OLDER) {
    c.execute(f2, f1);
  }
  else return;
  reload_panels();
  align_panels();
}

void Controller::touch(const std::string &name) {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  std::filesystem::path active_path = active.get_current_path() / name;
  bool ok = true;
  if (std::filesystem::exists(active_path))
    ok = FileGuard::confirm_overwrite(FileEntry(active_path));
  if (ok) {
    TouchOperation t;
    t.execute(FileEntry(active_path));  // <-- prima creo il file
    FileEntry fe(active_path);  // <-- poi costruisco FileEntry
    // Questo perché altrimenti la data non verrebbe settata
    if (sync_mode) {
      std::filesystem::path inactive_path = inactive.get_current_path() / name;
      t.execute(FileEntry(inactive_path));
      FileEntry inactive_fe(inactive_path);
    }
  }
}

void Controller::mkdir(const std::string &name) {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  std::filesystem::path new_dir = active.get_current_path() / name;
  if (std::filesystem::exists(new_dir)) {
    view.get_command_bar().print_message("Directory already exists: " + name,
                                         CommandBar::ERROR);
    return;
  }
  MkdirOperation m;
  m.execute(FileEntry(new_dir));
  FileEntry fe(new_dir);
  if (sync_mode)
    m.execute(FileEntry(inactive.get_current_path() / name));
}

void Controller::move_file(const std::string &name) {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  if (active.get_file_list().empty())
    return;
  const FileEntry &fe = active.get_current_file();
  if (fe.is_placeholder())
    return;
  FileEntry source = FileEntry(fe.get_path());
  FileEntry destination(active.get_current_path() / name);
  if (destination.exists()) {
    if (!FileGuard::confirm_overwrite(source, destination))
      return;
  }
  MoveOperation m;
  m.execute(source, destination);
  if (sync_mode) {
    FileEntry inactive_source =
    FileEntry(inactive.get_current_path() / source.get_name());
    FileEntry inactive_destination = FileEntry(inactive.get_current_path() / name);
    if (inactive_source.exists())
      m.execute(inactive_source, inactive_destination);
  }
}

void Controller::move_file() {
  if (sync_mode)
    return;
  // non posso usare std::filesystem::renamve (vedi MoveOperation)
  // perché rename non funziona su partizioni diverse
  // rename: Invalid cross-device link
  // quindi lo copio prima e poi lo cancello
  copy_file();
  delete_file(true);
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
    view.get_command_bar().print_message("Directory not exists",
                                         CommandBar::ERROR);
    return;
  }
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  active.change_dir(full_path);
  if (sync_mode) {
    inactive.change_dir(inactive.get_current_path() /
    std::filesystem::path(path).filename());
    align_panels();
  }
}
