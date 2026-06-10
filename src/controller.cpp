#include "controller.h"
#include "command.h"
#include "command_bar.h"
#include "copy_operation.h"
#include "delete_operation.h"
#include "file_entry.h"
#include "file_guard.h"
#include "file_comparator.h"
#include "mkdir_operation.h"
#include "move_operation.h"
#include "operation.h"
#include "panel.h"
#include "popup.h"
#include "touch_operation.h"
#include "view.h"
#include <filesystem>
#include <unistd.h>
#include <ranges>
#include <algorithm>

#define ctrl(x) ((x)&0x1f) // definisce CTRL+H

#include <fstream>
void Controller::test() {
  // Crea le cartelle
  std::filesystem::create_directories("/tmp/A");
  std::filesystem::create_directories("/tmp/B");
  
  // File in A
  // Nomi comuni (presenti in entrambe le cartelle) — 130 file
  std::vector<std::string> common = {
    "alpha.txt", "beta.txt", "gamma.txt", "delta.txt", "epsilon.txt",
    "zeta.txt", "eta.txt", "theta.txt", "iota.txt", "kappa.txt",
    "lambda.txt", "mu.txt", "nu.txt", "xi.txt", "omicron.txt",
    "pi.txt", "rho.txt", "sigma.txt", "tau.txt", "upsilon.txt",
    "phi.txt", "chi.txt", "psi.txt", "omega.txt",
    "report_01.txt", "report_02.txt", "report_03.txt", "report_04.txt", "report_05.txt",
    "report_06.txt", "report_07.txt", "report_08.txt", "report_09.txt", "report_10.txt",
    "data_001.csv", "data_002.csv", "data_003.csv", "data_004.csv", "data_005.csv",
    "data_006.csv", "data_007.csv", "data_008.csv", "data_009.csv", "data_010.csv",
    "log_2024_01.log", "log_2024_02.log", "log_2024_03.log", "log_2024_04.log",
    "log_2024_05.log", "log_2024_06.log", "log_2024_07.log", "log_2024_08.log",
    "config_a.json", "config_b.json", "config_c.json", "config_d.json",
    "config_e.json", "config_f.json", "config_g.json", "config_h.json",
    "file_001.bin", "file_002.bin", "file_003.bin", "file_004.bin", "file_005.bin",
    "file_006.bin", "file_007.bin", "file_008.bin", "file_009.bin", "file_010.bin",
    "file_011.bin", "file_012.bin", "file_013.bin", "file_014.bin", "file_015.bin",
    "file_016.bin", "file_017.bin", "file_018.bin", "file_019.bin", "file_020.bin",
    "backup_mon.tar", "backup_tue.tar", "backup_wed.tar", "backup_thu.tar",
    "backup_fri.tar", "backup_sat.tar", "backup_sun.tar",
    "img_001.png", "img_002.png", "img_003.png", "img_004.png", "img_005.png",
    "img_006.png", "img_007.png", "img_008.png", "img_009.png", "img_010.png",
    "doc_a.xml", "doc_b.xml", "doc_c.xml", "doc_d.xml", "doc_e.xml",
    "doc_f.xml", "doc_g.xml", "doc_h.xml", "doc_i.xml", "doc_j.xml",
    "cache_1.tmp", "cache_2.tmp", "cache_3.tmp", "cache_4.tmp", "cache_5.tmp",
    "cache_6.tmp", "cache_7.tmp", "cache_8.tmp", "cache_9.tmp", "cache_10.tmp",
    "index_a.idx", "index_b.idx", "index_c.idx", "index_d.idx", "index_e.idx",
    "note_jan.md", "note_feb.md", "note_mar.md", "note_apr.md",
    "note_may.md", "note_jun.md", "note_jul.md", "note_aug.md",
    "test_unit_01.cpp", "test_unit_02.cpp", "test_unit_03.cpp",
    "test_unit_04.cpp", "test_unit_05.cpp",
    "readme.md", "changelog.txt", "license.txt", "makefile", "cmakelists.txt",
  };
  
  // Solo in A — 25 file esclusivi
  std::vector<std::string> only_a = {
    "exclusive_a_01.txt", "exclusive_a_02.txt", "exclusive_a_03.txt",
    "exclusive_a_04.txt", "exclusive_a_05.txt", "exclusive_a_06.txt",
    "exclusive_a_07.txt", "exclusive_a_08.txt", "exclusive_a_09.txt",
    "exclusive_a_10.txt", "exclusive_a_11.txt", "exclusive_a_12.txt",
    "exclusive_a_13.txt", "exclusive_a_14.txt", "exclusive_a_15.txt",
    "only_in_a_1.dat", "only_in_a_2.dat", "only_in_a_3.dat",
    "only_in_a_4.dat", "only_in_a_5.dat",
    "snapshot_v1.bak", "snapshot_v2.bak", "snapshot_v3.bak",
    "draft_final.txt", "archive_old.zip",
  };
  
  // Solo in B — 25 file esclusivi
  std::vector<std::string> only_b = {
    "exclusive_b_01.txt", "exclusive_b_02.txt", "exclusive_b_03.txt",
    "exclusive_b_04.txt", "exclusive_b_05.txt", "exclusive_b_06.txt",
    "exclusive_b_07.txt", "exclusive_b_08.txt", "exclusive_b_09.txt",
    "exclusive_b_10.txt", "exclusive_b_11.txt", "exclusive_b_12.txt",
    "exclusive_b_13.txt", "exclusive_b_14.txt", "exclusive_b_15.txt",
    "only_in_b_1.dat", "only_in_b_2.dat", "only_in_b_3.dat",
    "only_in_b_4.dat", "only_in_b_5.dat",
    "release_v1.bak", "release_v2.bak", "release_v3.bak",
    "final_build.txt", "dist_package.zip",
  };
  
  for (const auto &name : common) {
    std::ofstream("/tmp/A/" + name);
    std::ofstream("/tmp/B/" + name);
  }
  for (const auto &name : only_a) {
    std::ofstream("/tmp/A/" + name);
  }
  for (const auto &name : only_b) {
    std::ofstream("/tmp/B/" + name);
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
  //test();
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

Panel& Controller::get_active_panel() {
  return panels[0].is_active() ? panels[0] : panels[1];
}

const Panel& Controller::get_active_panel() const {
  return panels[0].is_active() ? panels[0] : panels[1];
}

Panel& Controller::get_inactive_panel() {
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
      changed = true;
      break;
      
    case KEY_PPAGE:
      move(-1, view.get_height());
      changed = true;
      break;
      
    case KEY_UP:
      move(-1, 1);
      changed = true;
      break;
      
    case KEY_DOWN:
      move(1, 1);
      changed = true;
      break;
      
    case KEY_LEFT:
      go_left();
      changed = true;
      break;
      
    case KEY_RIGHT:
      go_right();
      changed = true;
      break;
      
    case KEY_HOME:
      go_first();
      changed = true;
      break;
      
    case KEY_END:
      go_last();
      changed = true;
      break;
      
    case KEY_ENTER:
    case 10:
    case 13:
      enter_pressed();
      break;
    case '-':
      go_up();
      changed = true;
      break;
      
    case ctrl('h'): {
      for (int i = 0; i < 2; i++) {
        panels.at(i).show_hidden_files(!panels.at(i).is_showing_hidden());
        panels.at(i).reload();
      }
      changed = true;
      break;
    }
    case ctrl('t'):
      toggle_tag_file();
      changed = true;
      break;
    case 9: // TAB
      change_active_panel();
      changed = true;
      break;
      
    case '/': {
      std::string str = get_command(CommandType::SEARCH);
      if (!str.empty())
        search_file(str);
      break;
    }
    
    case ':': {
      std::string cmd = get_command(CommandType::COMMAND);
      if (!cmd.empty())
        evaluate_command(cmd);
      break;
    }
    case '$': {
      reload_panels();
      if (sync_mode) {
        comparator.stop();
        align_panels();
        comparator.start(get_active_panel(), get_inactive_panel());
      }
      changed = true;
      break;
    }
    default:
      if(jump_to_file(ch))
      changed = true;
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
  if (changed) {
    panels[0].reload();
    panels[1].reload();
  }
}

void Controller::draw_panels() {
  if (changed) {
    view.draw_panels(sync_mode);
    changed = false;
  }
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
      comparator.stop();
      inactive_panel.change_dir(inactive_panel.get_current_path() / entry.get_name());
      align_panels();
      comparator.start(active_panel, inactive_panel);
    }
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

bool Controller::jump_to_file(char ch) {
  // auto oggetto = [capture list] (parametri) -> tipo_ritorno { corpo };
  auto jump = [&](Panel &p) -> bool {
    for (int i = p.get_selected_index() + 1;
         i < static_cast<int>(p.get_raw_file_list().size()); i++) {
      std::string name = p.get_file_at(i).get_name();
    if (name[0] == ch) {
      p.set_selected_index(i);
      view.set_offset(p, i);
      return true;
    }
         }
         
         for (int i = 0; i < p.get_selected_index(); i++) {
           std::string name = p.get_file_at(i).get_name();
           if (name[0] == ch) {
             p.set_selected_index(i);
             view.set_offset(p, i);
             return true;
           }
         }
         return false;
  }; // end lambda
  
  bool result = jump(get_active_panel());
  if (sync_mode)
    result |= jump(get_inactive_panel());
  return result;
}

void Controller::toggle_tag_file() {
  /* if (sync_mode) {
   *    CommandBar &c = view.get_command_bar();
   *    c.print_message("Tagging disabled in sync mode", CommandBar::ERROR);
   *    return;
}*/
  Panel &p1 = get_active_panel();
  Panel &p2 = get_inactive_panel();
  p1.toggle_tag_current_file();
  p1.move_down(1);
  if (sync_mode) {
    p2.toggle_tag_current_file();
    p2.move_down(1);
  }
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

void Controller::clear_command_bar() {
  view.clear_command_bar();
}
// ---------------------------------------------------------------------------
// Comandi
// ---------------------------------------------------------------------------

std::string Controller::get_command(CommandType c) {
  return view.get_command_bar().get_command(c);
}

void Controller::execute_command(const std::string& cmd) {
  def_prog_mode();      // salva lo stato del terminale ncurses
  endwin();             // sospendi ncurses, ripristina terminale normale
  
  std::system(cmd.c_str());  // qui cat/ls/ecc. scrivono normalmente
  
  printf("\nPress any key to continue...");
  getchar();
  
  reset_prog_mode();    // ripristina lo stato ncurses salvato
  refresh();            // ridisegna la UI
}

void Controller::search_file(const std::string &name) {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  const auto &files = active.get_file_list();
  
  for (int i = 0; i < (int)files.size(); i++) {
    if (files[i].get_name().find(name) != std::string::npos) {
      active.set_selected_index(i);
      if (sync_mode)
        inactive.set_selected_index(i);
      return;
    }
  }
  // nessun risultato
  view.get_command_bar().print_message("Not found: " + name, CommandBar::ERROR);
}

void Controller::evaluate_command(const std::string &cmd) {
  Command command(this);
  command.execute(cmd);
  for (int i = 0; i < 2; i++)
    panels[i].reload();
  if (sync_mode) {
    comparator.stop();        // ferma il precedente
    align_panels();
    for (int i = 0; i < 2; i++)
      panels[i].update_selected_index();
    comparator.start(get_active_panel(), get_inactive_panel());  // riparte
  }
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
    if (f.get_sync_status() == SyncStatus::NEWER || f.get_sync_status() == SyncStatus::OLDER || f.get_sync_status() == SyncStatus::ONCE || f.is_placeholder()) {
      active.set_selected_index(i);
      inactive.set_selected_index(i);
      return;
    }
  }
}

void Controller::set_sync(bool sync) {
  if (!sync) {
    comparator.stop();
    sync_mode = false;
    sync_partner(false);
    return;
  }
  sync_mode = true;
  sync_partner(true);
  align_panels();
  sync_index();
  //  comparator.start(get_active_panel(), get_inactive_panel());
}

void Controller::sort_different(bool sort) {}

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

void Controller::delete_backup_files() {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  
  auto files_active = active.get_file_list();
  auto files_inactive = sync_mode ? inactive.get_file_list() : std::vector<FileEntry>{};
  
  
  bool has_backups = std::ranges::any_of(files_active, &FileEntry::is_backup) ||
  std::ranges::any_of(files_inactive, &FileEntry::is_backup);
  
  if (!has_backups) return;
  comparator.stop();
  DeleteOperation d;
  
  for (const auto &f : files_active) {
    if (f.is_backup()) d.execute(f);
  }
  
  for (const auto &f : files_inactive) {
    if (f.is_backup()) d.execute(f);
  }
  
  active.clear_tagged_selection();
  inactive.clear_tagged_selection();
  reload_panels();
  if (sync_mode) {
    align_panels();
    comparator.start(get_active_panel(), get_inactive_panel());
  }
}

void Controller::delete_file(bool silent) {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  auto files_active = active.get_files_to_operate();
  auto files_inactive = sync_mode ? inactive.get_files_to_operate() : std::vector<FileEntry>{};
  
  if (files_active.empty() && files_inactive.empty())
    return;
  
  comparator.stop();
  DeleteOperation d;
  
  // file nell'attivo
  for (const auto &f : files_active) {
    FileEntry inactive_file(inactive.get_current_path() / f.get_name());
    bool has_pair = sync_mode && inactive_file.exists();
    if (!silent) {
      bool ok = has_pair ? FileGuard::confirm_delete(f, inactive_file)
      : FileGuard::confirm_delete(f);
      if (!ok) continue;
    }
    d.execute(f);
    if (has_pair)
      d.execute(inactive_file);
  }
  
  // file solo nell'inattivo (placeholder nell'attivo)
  for (const auto &f : files_inactive) {
    FileEntry active_file(active.get_current_path() / f.get_name());
    if (!active_file.exists()) {
      if (!silent) {
        bool ok = FileGuard::confirm_delete(f);
        if (!ok) continue;
      }
      d.execute(f);
    }
  }
  
  active.clear_tagged_selection();
  inactive.clear_tagged_selection();
  reload_panels();
  if (sync_mode) {
    align_panels();
    comparator.start(get_active_panel(), get_inactive_panel());
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
  p1.clear_tagged_selection();
}

void Controller::sync_file() {
  Panel &active = get_active_panel();
  Panel &inactive = get_inactive_panel();
  auto files_active = active.get_files_to_operate();
  auto files_inactive = inactive.get_files_to_operate();
  
  if (files_active.empty() && files_inactive.empty())
    return;
  
  comparator.stop();
  CopyOperation c;
  
  // file nell'attivo → copia nell'inattivo
  for (const auto &f1 : files_active) {
    FileEntry f2(inactive.get_current_path() / f1.get_name());
    if (!f2.exists()) {
      c.execute(f1, f2);
    } else if (f1.get_sync_status() == SyncStatus::NEWER) {
      c.execute(f1, f2);
    } else if (f1.get_sync_status() == SyncStatus::OLDER) {
      c.execute(f2, f1);
    }
    // SAME → niente da fare
  }
  
  // file solo nell'inattivo (placeholder nell'attivo)
  for (const auto &f2 : files_inactive) {
    FileEntry f1(active.get_current_path() / f2.get_name());
    if (!f1.exists())
      c.execute(f2, f1);
  }
  active.clear_tagged_selection();
  inactive.clear_tagged_selection();
  reload_panels();
  align_panels();
  comparator.start(get_active_panel(), get_inactive_panel());
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
