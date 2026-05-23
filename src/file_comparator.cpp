/**
 * Logica di sincronizzazione tra pannelli
 * ========================================
 *
 * 1. CARICAMENTO (Panel::reload)
 *    - raw_file_list viene popolata con i file del filesystem
 *    - tutti i file hanno SyncStatus::UNKNOWN (non ancora confrontati)
 *
 * 2. ALLINEAMENTO (Panel::align_with)
 *    - aligned_file_list viene creata da raw_file_list
 *    - file presenti in entrambi i pannelli → UNKNOWN (da confrontare)
 *    - file presenti in un solo pannello   → ONCE   (già definitivo)
 *    - buchi (placeholder)                 → nullptr/FileEntry vuoto
 *
 * 3. CONFRONTO IN BACKGROUND (FileComparator::compare_all)
 *    - itera su aligned_file_list
 *    - salta placeholder e file già confrontati (!= UNKNOWN)
 *    - per ogni file UNKNOWN chiama compare_files che imposta:
 *      SAME   → file identici (stessa dimensione e hash)
 *      NEWER  → questo file è più nuovo
 *      OLDER  → questo file è più vecchio
 *
 * 4. DOPO UN COMANDO (Controller::evaluate_command)
 *    - reload()            → rilegge il filesystem
 *    - comparator.stop()   → ferma il thread precedente
 *    - align_panels()      → riallinea con UNKNOWN/ONCE
 *    - comparator.start()  → riparte il confronto in background
 */

#include "file_comparator.h"
#include "file_entry.h"
#include <fstream>
#include <vector>
#include <algorithm>

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
    auto& files_a = panel_a->get_file_list();
    auto& files_b = panel_b->get_file_list();
    
    for (int i = 0; i < (int)files_a.size(); i++) {
        if (!running) return;
        if (files_a[i].is_placeholder() || files_b[i].is_placeholder()) continue;  // ← già ONCE
        if (files_a[i].get_sync_status() != SyncStatus::UNKNOWN) continue;  // ← già confrontato
        compare_files(files_a[i], files_b[i]);
    }
}

void FileComparator::compare_files(FileEntry &a, FileEntry &b) {
  if (a.get_sync_status() == SyncStatus::DIR || b.get_sync_status() == SyncStatus::DIR)
    return;
  
  SyncStatus status_a = SyncStatus::UNKNOWN;
  SyncStatus status_b = SyncStatus::UNKNOWN;
  
  try {
    auto size_a = std::filesystem::file_size(a.get_path());
    auto size_b = std::filesystem::file_size(b.get_path());
    
    if (size_a == size_b && a.get_name() == b.get_name()) {
      status_a = SyncStatus::SAME;
      status_b =  SyncStatus::SAME;
    }
    else if (size_a != size_b) {
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
