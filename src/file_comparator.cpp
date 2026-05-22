#include "file_comparator.h"
#include <fstream>
#include <vector>

void FileComparator::start(Panel& a, Panel& b) {
  panel_a = &a;
  panel_b = &b;
  running = true;
    thread = std::thread([this]() {
      compare_all();
    });
}

void FileComparator::stop() {
  running = false;
  //aspetta che il thread si arresti
  if (thread.joinable()) thread.join();
}

void FileComparator::compare_all() {
    // i files dei pannelli devono essere allineati,
  // quindi si presuppone abbiano la stessa dimensione.
  for (int i = 0; i < panel_a->get_file_list().size(); i++) {
  if (!running) return; // fermati se stop() è stato chiamato
    compare_files(panel_a->get_file_list().at(i), panel_b->get_file_list().at(i));
  }
}

#include "iostream"
 void FileComparator::compare_files(FileEntry &a, FileEntry &b) {
//  std::cerr << "Compare " << a.get_name() << " with " << b.get_name() << std::endl;
   if (a.get_sync_status() == SyncStatus::DIR || b.get_sync_status() == SyncStatus::DIR)
    return;

  SyncStatus status_a, status_b;

  try {
    auto size_a = std::filesystem::file_size(a.get_path());
    auto size_b = std::filesystem::file_size(b.get_path());
    if (size_a != size_b) {
      auto time_a = std::filesystem::last_write_time(a.get_path());
      auto time_b = std::filesystem::last_write_time(b.get_path());
      status_a = (time_a > time_b) ? SyncStatus::NEWER : SyncStatus::OLDER;
      status_b = (time_a > time_b) ? SyncStatus::OLDER : SyncStatus::NEWER;
    } else {
      auto hash_a = partial_hash(a.get_path(), 8192);
      auto hash_b = partial_hash(b.get_path(), 8192);
      if (hash_a == hash_b) {
        status_a = status_b = SyncStatus::SAME;
      } else {
        auto time_a = std::filesystem::last_write_time(a.get_path());
        auto time_b = std::filesystem::last_write_time(b.get_path());
        status_a = (time_a > time_b) ? SyncStatus::NEWER : SyncStatus::OLDER;
        status_b = (time_a > time_b) ? SyncStatus::OLDER : SyncStatus::NEWER;
      }
    }
  } catch (const std::filesystem::filesystem_error &) {
    status_a = status_b = SyncStatus::NONE;
  }

  // aggiorna solo alla fine con il lock
  std::scoped_lock lock(panel_a->get_mutex(), panel_b->get_mutex());
  a.set_sync_status(status_a);
  b.set_sync_status(status_b);
}

size_t FileComparator::partial_hash(const std::filesystem::path &path, size_t bytes) {
  std::ifstream f(path, std::ios::binary);
  if (!f)
    return 0;
  std::vector<char> buf(bytes);
  f.read(buf.data(), bytes);
  size_t read = f.gcount();
  return std::hash<std::string_view>{}(std::string_view(buf.data(), read));
}
