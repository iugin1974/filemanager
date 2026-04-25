#include "controller.h"
#include "view.h"
#include "panel.h"
#include "command_bar.h"
#include "command.h"

Controller::Controller(View& view) : view(view) {
  panels[0].reload();
  panels[1].reload();
  panels[0].set_active(true);
  view.init_panels(&panels[0], &panels[1]);
}

void Controller::handle_key(int ch) {
  switch (ch) {
    case KEY_UP:
      panels[get_active_panel()].move_up();
      break;
    case KEY_DOWN:
      panels[get_active_panel()].move_down();
      break;
    case KEY_LEFT:
      go_up();
      break;
    case ':': {
		std::string cmd = get_command();
		evaluate_command(cmd);
		break;
	      }
    case 9: // TAB
	      change_active_panel();
	      break;
    case KEY_ENTER:
    case 10:
    case 13:
	      enter_pressed(panels[get_active_panel()].get_selected_index());
	      break;
    default:
	      break;
  }
  view.draw_panels();
}

void Controller::enter_pressed(int selected_line) {
  auto& panel = panels[get_active_panel()];
  FileEntry file_entry =  panel.get_file(selected_line);
  std::filesystem::path path = file_entry.get_path();
  if (file_entry.is_directory()) {
    panel.change_dir(path);
    view.draw_panels(); 
  }
}

void Controller::go_up() {
  auto& panel = panels[get_active_panel()];
  bool ok = panel.go_up();
  if (!ok) return;
  view.draw_panels();
}

std::string Controller::get_command() {
  auto& cb = view.get_command_bar(get_active_panel());
  return cb.get_command();
}

void Controller::evaluate_command(std::string cmd) {
  Command c;
  Command::Action command = c.parse_command(cmd);
  if (command == Command::Action::OpenPanel) {
  }
}

int Controller::get_active_panel() {
  if (panels[0].is_active()) return 0;
  return 1;
}

void Controller::change_active_panel() {
  for (int i = 0; i < 2; i++) {
    panels[i].set_active(!panels[i].is_active());
  }
}
