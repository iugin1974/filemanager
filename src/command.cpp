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
    else if (name == "mv" && args.size() == 0)
        controller->move_file();
    else if (name == "mv" && args.size() == 1)
        controller->move_file(args[0]);
}


std::vector<std::string> Command::parse_command(const std::string& cmd) {
    std::vector<std::string> tokens;
    std::string current;
    bool in_quotes = false;
    
    for (char c : cmd) {
        
        if (c == '"') {
            in_quotes = !in_quotes;
            continue; // non salvare le virgolette
        }
        
        if (std::isspace(c) && !in_quotes) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        }
        else {
            current += c;
        }
    }
    
    if (!current.empty()) {
        tokens.push_back(current);
    }
    
    return tokens;
}
