#pragma once
#include <vector>
#include "panel.h"

class View;  // forward declaration

class Controller {
  public:
    Controller(View& view);
    void handle_key(int ch);

  private:
    
    Panel panels[2];
    View& view;
    int active_panel = 0;
    void enter_pressed(int selected_line);
    void go_up();
    std::string get_command();
    void evaluate_command(std::string);
    int get_active_panel();
    void change_active_panel();
};
