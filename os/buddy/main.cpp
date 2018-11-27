#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <cassert>
#include <string>

#include <readline/readline.h>

#include "buddy_proxy.h"
#include "clrprintf.h"

int main(int argc, const char *argv[]) {
  std::cout << "Buddy Allocator Simulator with 16384KiB Memory" << std::endl;

  BuddyAllocator buddyAllocator { 16384 };
  BuddyProxy proxy(buddyAllocator);

  const char *kdstr;
  while ((kdstr = readline("Simulator> "))) {
    std::stringstream ss;
    std::string tmp;

    ss << kdstr;
    ss >> tmp;

    if (tmp == "malloc" || tmp == "m") {
      ss >> tmp;
      try {
        proxy.malloc(std::stoi(tmp, 0, 0));
      } catch (std::invalid_argument &ex) {
        clrprintf(CLR_FAILED, "Please enter the size of malloc.\n");
      }
    } else if (tmp == "free" || tmp == "f") {
      ss >> tmp;
      try {
        proxy.free(std::stoi(tmp, 0, 0));
      } catch (std::invalid_argument &ex) {
        clrprintf(CLR_FAILED, "Please enter the pointer want to free.\n");
      }
    } else if (tmp == "print" || tmp == "p") {
      proxy.printZonelistStatus();
    } else if (tmp == "quit" || tmp == "q") {
      break;
    } else if (tmp == "help" || tmp == "h") {
      clrprintf(CLR_MESSAGE, R"(Command List:
    malloc / m: To malloc memory
    free   / f: free memory that malloced
    print  / p: show linklist status
    help   / h: show usage of simualtor
    quit   / q: quit simulator
)");

    } else {
      clrprintf(CLR_FAILED, "Cannot realize what command mean.\n");
    }
  }
  return 0;
}