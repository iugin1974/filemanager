#pragma once
#include <vector>
#include "panel.h"

class View;  // forward declaration

class Controller {
  public:
    Controller(View& view);
    void add_panel();
    void handle_key(int ch);
  private:
    std::vector<Panel> panels;
    View& view;
    int active_panel = 0;
    void enter_pressed(int selected_line);
    void go_up();
};
