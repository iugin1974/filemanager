cmake_minimum_required(VERSION 3.16)

project(FileManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/app.cpp
    src/command_bar.cpp
    src/command.cpp
    src/controller.cpp
    src/copy_operation.cpp
    src/delete_operation.cpp
    src/file_entry.cpp
    src/filesystem.cpp
    src/main.cpp
    src/mkdir_operation.cpp
    src/move_operation.cpp
    src/panel.cpp
    src/panel_view.cpp
    src/popup.cpp
    src/rename_operation.cpp
    src/status_bar.cpp
    src/view.cpp
)

add_executable(filemanager ${SOURCES})

find_package(Curses REQUIRED)

target_include_directories(filemanager PRIVATE
    src
    ${CURSES_INCLUDE_DIR}
)

target_link_libraries(filemanager PRIVATE
    ${CURSES_LIBRARIES}
)
