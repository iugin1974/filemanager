#pragma once

#include <string>

class Command {

  public:
    enum class Action {
        None,
        OpenPanel,
        ClosePanel
    };
Action parse_command(std::string cmd);
  private:

};
