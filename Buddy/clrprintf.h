#ifndef BUDDY_SYSTEM_CLRPRINTF_H
#define BUDDY_SYSTEM_CLRPRINTF_H

#include <cstdio>
#include <cstdarg>

#define CLR_SUCCEED "\033[32m"
#define CLR_FAILED  "\033[31m"
#define CLR_MESSAGE "\033[34m"
#define CLR_TITLE   "\033[33m"
#define CLR_UNSET   "\033[0m"

inline void clrprintf(const char *clr, const char *format, ...) {
  printf(clr);
  va_list va;
  va_start(va, format);
  vprintf(format, va);
  va_end(va);
  printf(CLR_UNSET);
}

#endif //BUDDY_SYSTEM_CLRPRINTF_H
