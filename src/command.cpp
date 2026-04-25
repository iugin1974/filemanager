#include "command.h"
#include <string>

Command::Action Command::parse_command(std::string cmd) {
  if (cmd == "new") {
    return Action::OpenPanel;
  }
return Action::None;
}
