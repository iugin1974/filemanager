#pragma once

#include "view.h"
#include "panel.h"
#include <string>
#include <array>

class Controller {
  public:
    explicit Controller(View& view);

    void handle_key(int ch);

    void set_sync(bool);
  private:
    // --- Stato ---
    View&              view;
    std::array<Panel, 2> panels;
    bool               sync_mode = true;

    // --- Helpers ---
    int  get_active_panel() const;
    void change_active_panel();

    template <typename Fn>
      void for_active_panels(Fn fn);

    // --- Azioni ---
    void enter_pressed(int selected_line, int panel_index);
    void go_up();

    // --- Comandi ---
    std::string get_command();
    void        evaluate_command(const std::string& cmd);
};
