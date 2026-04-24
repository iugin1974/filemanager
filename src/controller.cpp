#include "controller.h"
#include "view.h"
#include "panel.h"

Controller::Controller(View& view) : view(view), active_panel(0) {}

void Controller::add_panel() {
  panels.emplace_back();
  panels.back().reload();
  view.add_panel(panels.back());
}

void Controller::handle_key(int ch) {
  switch (ch) {
    case KEY_UP:
      panels[active_panel].move_up();
      break;
    case KEY_DOWN:
      panels[active_panel].move_down();
      break;
    case KEY_LEFT:
      go_up();
      break;
    case 9: // TAB
      active_panel = (active_panel + 1) % panels.size();
      break;
    case KEY_ENTER:
    case 10:
    case 13:
      enter_pressed(panels.at(active_panel).get_selected_index());
      break;
    default:
      break;
  }
  view.draw_panels();
}

void Controller::enter_pressed(int selected_line) {
  auto& panel = panels.at(active_panel);
  FileEntry file_entry = panel.get_file(selected_line);
  std::filesystem::path path = file_entry.get_path();
  if (file_entry.is_directory()) {
    panel.change_dir(path);
    view.draw_panels(); 
  }
}

void Controller::go_up() {
  auto& panel = panels.at(active_panel);
  bool ok = panel.go_up();
  if (!ok) return;
  view.draw_panels();
}
