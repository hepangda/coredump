#ifndef BUDDY_SYSTEM_MEMORY_ZONE_LIST_H
#define BUDDY_SYSTEM_MEMORY_ZONE_LIST_H

#include <list>

#include "buddy_types.h"

class MemoryZoneList : public std::list<mem_zone_t> {
 public:
  memptr_t takeSequence(int blocks);
  memptr_t take();

  void autoInsert(memptr_t addr);
  void splitTo(MemoryZoneList &other, int other_level);
};

#endif //BUDDY_SYSTEM_MEMORY_ZONE_LIST_H
