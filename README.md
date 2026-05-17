# fm — Terminal File Manager

A dual-panel terminal file manager written in C++17, built on ncurses.
Inspired by classic tools like Midnight Commander, with a focus on directory
synchronization.

![Platform](https://img.shields.io/badge/platform-Linux-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![License](https://img.shields.io/badge/license-MIT-green)

---

## Features

- Dual-panel layout
- Navigation history with back/forward (like a browser)
- **Sync mode**: compare two directories side by side
  - Files are aligned by name
  - Newer files highlighted in green, older in red
  - Jump directly to the next difference
  - Sync a single file or copy missing files across panels
- File operations: copy, move, rename, delete, touch, mkdir
- Multi-file tagging
- Hidden files toggle
- Command bar for quick actions

## Requirements

- Linux
- CMake >= 3.16
- A C++17 compiler (GCC or Clang)
- ncurses

On Debian/Ubuntu:
```bash
sudo apt install cmake g++ libncurses-dev
```

On Arch:
```bash
sudo pacman -S cmake gcc ncurses
```

## Build

```bash
git clone https://github.com/yourusername/fm.git
cd fm
mkdir build && cd build
cmake ..
make
```

## Install

```bash
sudo make install
```

This installs `fm` to `/usr/bin/fm`.

## Usage

```bash
fm
```

Use **Tab** to switch between panels.

## Key Bindings

### Navigation

| Key | Action |
|-----|--------|
| `↑` `↓` | Move up/down |
| `Page Up` `Page Down` | Scroll by page |
| `Home` `End` | Jump to first/last entry |
| `Enter` | Open directory or file |
| `←` | Go back in history |
| `→` | Go forward in history |
| `-` | Go to parent directory |

### File Operations

All operations are entered via the command bar, opened with `:`

| Command | Action |
|---------|--------|
| `:cp` | Copy selected file to the other panel |
| `:mv` | Move or rename selected file |
| `:rm` | Delete selected file |
| `:touch <filename>` | Create an empty file |
| `:mkdir A B C` | Create one or more directories |
| `:cd <path>` | Change directory |

### Tagging

| Key | Action |
|-----|--------|
| `Ctrl+T` | Tag/untag selected file |

Tagged files are marked with `*` and used as the source for file operations.

### Sync Mode

| Command / Key | Action |
|---------------|--------|
| `:sync on` | Enable sync mode |
| `:sync off` | Disable sync mode |
| `:j` | Jump to next different file |
| `:cp` | Copy newer file over older one |
| `:rm` | Delete file from both panels |
| `:mkdir A B C` | Create directories in both panels |

In sync mode, files are aligned by name across both panels:

- 🟢 **Green** — this file is newer
- 🔴 **Red** — this file is older
- White — files are identical
- Empty slot — file exists only in one panel

### Other

| Key | Action |
|-----|--------|
| `Ctrl+H` | Toggle hidden files |
| `:` | Open command bar |
| `:q` | Quit |

## Project Structure

```
src/
├── app.cpp / app.h           # Entry point, ncurses init
├── controller.cpp / .h       # Input handling, business logic
├── view.cpp / .h             # Rendering
├── panel.cpp / .h            # Panel state (path, file list, history)
├── panel_view.cpp / .h       # Panel rendering
├── file_entry.cpp / .h       # File metadata and display
├── history.cpp / .h          # Navigation history
├── copy_operation.cpp / .h   # Copy
├── move_operation.cpp / .h   # Move/rename
├── delete_operation.cpp / .h # Delete
├── mkdir_operation.cpp / .h  # Create directory
├── touch_operation.cpp / .h  # Create file
├── file_guard.cpp / .h       # Confirmation dialogs
├── command_bar.cpp / .h      # Command input
├── status_bar.cpp / .h       # Status display
└── popup.cpp / .h            # Popup dialogs
```

## License

MIT
