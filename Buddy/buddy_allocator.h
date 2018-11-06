#ifndef BUDDY_SYSTEM_BUDDY_ALLOCATOR_H
#define BUDDY_SYSTEM_BUDDY_ALLOCATOR_H

#include <vector>

#include "buddy_types.h"
#include "memory_zone_list.h"

class BuddyAllocator {
 public:
  explicit BuddyAllocator(memsz_t memory_amount) : memory_amount_(memory_amount) {}

  memptr_t malloc(unsigned long long bytes) { return malloc_impl(byte2Block(bytes)); }
  void free(memptr_t start);
  void build();

 private:
  friend class BuddyProxy;

  std::vector<mem_page_frame_t> frame_;
  MemoryZoneList pool_[kMemlistSize()];
  memsz_t memory_amount_; // 4KiB

  memptr_t malloc_impl(memsz_t size);
  memptr_t malloc_impl_multi(memsz_t size);
  memptr_t malloc_impl_simple(memsz_t size);

  void mark(memptr_t ptr, memsz_t size);
  void merge(int tier);

  constexpr memsz_t fitSize(memsz_t size) {
    for (int i = 0; i <= 5; i++) {
      size |= size >> kMemorderPow2(i);
    }
    return size + 1;
  }
  constexpr bool isPowerOf2(memsz_t num) {
    return (num & (num - 1)) == 0;
  }
  constexpr int sizeToLevel(memsz_t size) {
    if (size >= kMemorderPow2(kMemlistSize())) return kMemlistSize() - 1;
    for (int i = kMemlistSize() - 1; i >= 0; i--) {
      if (size >= kMemorderPow2(i)) return i - 1;
    }

    return static_cast<int>(kMemFailed());
  }
  constexpr memsz_t byte2Block(unsigned long bytes) {
    return (bytes / 4) + (((bytes / 4.0) > static_cast<int>(bytes / 4)) ? 1 : 0);
  }
};

#endif //BUDDY_SYSTEM_BUDDY_ALLOCATOR_H
