#ifndef BANKER_OS_H
#define BANKER_OS_H

#include <mutex>
#include <vector>
#include <set>

using uint = unsigned int;

class Process;

class OS {
 public:
  OS(uint max = 0): available_(max), max_available_(max) {}

  void set_max_avaiable(uint max) { max_available_ = max; }
  void set_available(uint num) { available_ = num; }
  uint max_available() { return max_available_; }
  uint available() { return available_; }

  uint attainResource(Process *process);
  bool freeResource(Process *process);

  void printList();
 private:
  uint max_available_;
  uint available_;

  std::set<Process *> process_list_;
  std::vector<uint> process_id_;
  std::mutex mutex_;

  bool banker(Process &process);
};

#endif // BANKER_OS_H