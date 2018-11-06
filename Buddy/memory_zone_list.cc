#include <stdexcept>
#include <cassert>

#include "memory_zone_list.h"

memptr_t MemoryZoneList::takeSequence(int blocks) {
  if (this->size() < blocks) return kMemFailed();

  auto interval = kMemorderPow2(kMemlistSize());
  for (auto it = this->cbegin(); it != this->cend(); ++it) {
    auto store = it, jt = it;
    for (int i = 0; i < blocks; i++) {
      if (*jt != *(++jt) + interval) {
        break;
      } else if (i == blocks - 1) {
        memptr_t res = *store;
        auto kt = store;
        for (int k = 0; k < blocks; k++) {
          kt = this->erase(kt);
        }

        return res;
      }
    }
  }

  return kMemFailed();
}

memptr_t MemoryZoneList::take() {
  if (this->empty()) {
    throw std::logic_error("can\'t take from empty.");
  }
  memptr_t store = this->front();
  this->pop_front();
  return store;
}

void MemoryZoneList::autoInsert(memptr_t addr) {
  for (auto it = this->cbegin(); it != this->cend(); ++it) {
    if (*it > addr) {
      this->emplace(it, addr);
      return;
    }
  }
  this->emplace(cend(), addr);
}

void MemoryZoneList::splitTo(MemoryZoneList &other, int other_level) {
  assert(!this->empty());

  int level_size = kMemorderPow2(other_level);
  memptr_t addr = this->take();
  other.autoInsert(addr);
  other.autoInsert(addr + level_size);
}