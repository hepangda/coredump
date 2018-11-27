#ifndef BUDDY_SYSTEM_BUDDY_PROXY_H
#define BUDDY_SYSTEM_BUDDY_PROXY_H

#include <iosfwd>

#include "buddy_allocator.h"

class BuddyProxy {
 public:
  explicit BuddyProxy(BuddyAllocator &allocator): ori_(allocator) {
    allocator.build();
  }

  memptr_t malloc(int bytes);
  void free(memptr_t p);

  void printZonelistStatus();
 private:
  BuddyAllocator &ori_;
};

#endif //BUDDY_SYSTEM_BUDDY_PROXY_H
