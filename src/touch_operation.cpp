#include "touch_operation.h"
#include <filesystem>
#include <fstream>

void TouchOperation::execute(const std::filesystem::path& path) {
    std::ofstream file(path);    
}

void TouchOperation::execute(const std::filesystem::path&,
                              const std::filesystem::path&) {
    // non usato per touch
                              }
                              
