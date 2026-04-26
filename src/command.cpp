#include "command.h"
#include <string>
#include <vector>
#include <sstream>
#include "controller.h"

Command::Command(Controller* c) {
  controller = c;
}

void Command::execute(const std::string& cmd) {
  auto tokens = parse_command(cmd);
  if (tokens.empty()) return;
  
  std::string name = tokens[0];
  std::vector<std::string> args(tokens.begin() + 1, tokens.end());
  
  if (name == "sync") {
    if (args[0] == "on") controller->set_sync(true);
    else if (args[0] == "off") controller->set_sync(false);
  }
  else if (name == "rm") controller->delete_file();
  else if (name == "cp") controller->copy_file();
  else if (name == "mkdir" && args.size() > 0) {
   for (int i = 0; i < args.size(); i++) {
    controller->mkdir(args[i]); 
   }
  }
}


std::vector<std::string> Command::parse_command(const std::string& cmd) {
  std::vector<std::string> tokens;
  std::istringstream ss(cmd);
  std::string token;
  while (ss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}
