#pragma once

#include <string>
#include <vector>
#include "controller.h"

class Command {

  public:
    Command(Controller*);
    void execute(const std::string& cmd);
  private:
    std::vector<std::string> parse_command(const std::string& cmd);
    std::vector<std::string> command;
    Controller* controller;
};
