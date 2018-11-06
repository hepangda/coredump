#include <cstdio>
#include <stdexcept>
#include <cstdarg>

#include "buddy_proxy.h"
#include "clrprintf.h"

void BuddyProxy::printZonelistStatus() {
  clrprintf(CLR_TITLE, "[Zonelist Status]\n");

  for (int i = 0; i < kMemlistSize(); i++) {
    clrprintf(CLR_MESSAGE, "\t#%d\t [S] -> ", i);
    for (auto it = ori_.pool_[i].cbegin(); it != ori_.pool_[i].cend(); ++it) {
      clrprintf(CLR_MESSAGE, "%ld -> ", *it);
    }
    clrprintf(CLR_MESSAGE, "[E]\n");
  }
}

memptr_t BuddyProxy::malloc(int bytes) {
  clrprintf(CLR_TITLE, "<Malloc> Allocate %d bytes.\n", bytes);

  if (bytes <= 4) {
    clrprintf(CLR_FAILED, "\t[FAILED] Request too small.\n");
    return kMemFailed();
  }
  try {
    auto ptr = ori_.malloc(bytes);
    clrprintf(CLR_SUCCEED, "\t[SUCCEED] Allocated %d bytes of 0x%x.\n", bytes, ptr);
    return ptr;
  } catch (std::runtime_error &ex) {
    clrprintf(CLR_FAILED, "\t[FAILED] Bad alloc, there\'s no free space to allocate.\n");
  }

  return kMemFailed();
}

void BuddyProxy::free(memptr_t p) {
  clrprintf(CLR_TITLE, "<Free> On position 0x%x.\n", p);

  try {
    ori_.free(p);
    clrprintf(CLR_SUCCEED, "\t[SUCCEED] Free memory space on 0x%x.\n", p);
  } catch (std::runtime_error &ex) {
    clrprintf(CLR_FAILED, "\t[FAILED] Pointer isn\'t malloc-ed.\n");
  }
}