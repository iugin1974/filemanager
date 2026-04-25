cmake_minimum_required(VERSION 3.15)
project(filemanager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
find_package(Curses REQUIRED)

add_executable(filemanager
    src/main.cpp
    src/app.cpp
    src/panel.cpp
    src/panel_view.cpp
    src/view.cpp
    src/controller.cpp
    src/file_entry.cpp
    src/status_bar.cpp
    src/command.cpp
    src/command_bar.cpp
)

target_include_directories(filemanager PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(filemanager ${CURSES_LIBRARIES})
