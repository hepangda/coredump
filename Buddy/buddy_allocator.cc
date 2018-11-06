#include <stdexcept>
#include <cassert>
#include "buddy_allocator.h"

void BuddyAllocator::free(memptr_t start) {
  if (frame_[start].prev != &frame_[start])
    throw std::runtime_error("pointer dismatch");

  auto myblock = frame_[start].prev;

  int size = 0;
  for (; frame_[size + start].prev == myblock; size++) {
    frame_[size + start].prev = nullptr;
  }

  int insert_from = 0;
  for (int i = 1; i <= kMemlistSize(); ++i) {
    if (size >= kMemorderPow2(i)) insert_from++;
  }

  assert(!kDiviedBy(size, kMemorderPow2(insert_from)));
  int interval = kMemorderPow2(insert_from);
  for (auto i = start; i < size; i += interval) {
    pool_[insert_from].autoInsert(i);
  }

  for (int i = insert_from; i < kMemlistSize() - 1; i++) {
    merge(i);
  }
}

void BuddyAllocator::merge(int tier) {
  int tier_size = kMemorderPow2(tier);
  int left = 0, right = tier_size;

  for (auto it = pool_[tier].cbegin(); it != pool_[tier].cend();) {
    while (*it > left) {
      left += tier_size;
      right += tier_size;
    }

    if (*it == left) {
      auto t = it, next = ++t;
      if (next == pool_[tier].cend()) break;
      if (*next == right) {
        it = pool_[tier].erase(it);
        it = pool_[tier].erase(it);
        pool_[tier + 1].autoInsert(left);
      }
    } else {
      ++it;
    }
  }
}

void BuddyAllocator::build() {
  if (memory_amount_ / 4.0 != static_cast<int>(memory_amount_ / 4)) {
    throw std::invalid_argument("memory amount must be times of 4KiB.");
  }

  auto blocks = memory_amount_ / 4;
  frame_.resize(blocks);

  memptr_t baseaddr = 0;
  auto remain = blocks;
  for (auto tier = kMemlistSize(); remain > 0; --tier) {
    auto tier_size = kMemorderPow2(tier);
    while (remain >= tier_size) {
      pool_[tier - 1].autoInsert(baseaddr);
      baseaddr += tier_size;
      remain -= tier_size;
    }
  }
}

memptr_t BuddyAllocator::malloc_impl(memsz_t size) {
  if (size == 0) return kMemFailed();

  if (!isPowerOf2(size))
    size = fitSize(size);

  auto ptr = (size > kMemorderPow2(kMemlistSize())) ?
             malloc_impl_multi(size) :
             malloc_impl_simple(size);

  mark(ptr, size);
  return ptr;
}

memptr_t BuddyAllocator::malloc_impl_multi(memsz_t size) {
  assert(size > kMemorderPow2(kMemlistSize()));

  int need_blocks = static_cast<int>(size / kMemorderPow2(kMemlistSize()));
  return pool_[kMemlistSize()].takeSequence(need_blocks);
}

memptr_t BuddyAllocator::malloc_impl_simple(memsz_t size) {
  int level = sizeToLevel(size);
  if (!pool_[level].empty()) return pool_[level].take();

  for (int i = level + 1; i < kMemlistSize(); i++) {
    if (!pool_[i].empty()) {
      for (int j = i - 1; j >= level; j--) {
        pool_[j + 1].splitTo(pool_[j], j);
      }

      assert(!pool_[level].empty());
      return pool_[level].take();
    }
  }

  return kMemFailed();
}

void BuddyAllocator::mark(memptr_t ptr, memsz_t size) {
  if (ptr == kMemFailed()) {
    throw std::runtime_error("bad alloc");
  }

  for (auto i = 0; i < size; i++) {
    frame_[ptr + i].prev = &frame_[ptr];
  }
}