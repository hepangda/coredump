#ifndef BUDDY_SYSTEM_BUDDY_TYPES_H
#define BUDDY_SYSTEM_BUDDY_TYPES_H

#include <cstdint>

struct mem_page_frame_t {
  mem_page_frame_t *prev = nullptr;

  explicit mem_page_frame_t(mem_page_frame_t *prev = nullptr) : prev(prev) {}
};

using mem_zone_t = intptr_t;
using memsz_t = unsigned long long;
using memptr_t = intptr_t;

constexpr intptr_t kMempageSize() { return 4096; }
constexpr int kMemlistSize() { return 12; }
constexpr int kMemorderPow2(int e) { return e == 0 ? 1 : 2 * kMemorderPow2(e - 1); }

template<typename T = memsz_t>
constexpr memsz_t kMemFailed() { return static_cast<T>(-1); }

constexpr bool kDiviedBy(int x, int y) { return (static_cast<double>(x) / y) - static_cast<int>(x / y) > 0; }

#endif //BUDDY_SYSTEM_BUDDY_TYPES_H
