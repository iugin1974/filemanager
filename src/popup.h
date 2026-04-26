// popup.h
#pragma once
#include <ncurses.h>
#include <vector>
#include <string>

class Popup {
  public:
    static int show(std::vector<std::string> lines, std::vector<std::string> keys);
    Popup() = delete;
};
