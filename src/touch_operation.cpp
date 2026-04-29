#include "touch_operation.h"
#include <filesystem>
#include <fstream>

void TouchOperation::execute(const std::filesystem::path& path) {
    std::ofstream file(path);    
}

void TouchOperation::execute(const std::filesystem::path& path,
                              const std::filesystem::path& path2) {
    // non usato per touch
                              }
                              
