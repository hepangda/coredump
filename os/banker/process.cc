#include "os.h"
#include "process.h"

bool Process::requestResource(uint num, OS &sys) {
  if (num <= max_need() - own_need()) {
    if (!set_request_need(num)) return false;

    int res = sys.attainResource(this);
    return (res == 0) ? true : false;
  }

  return false;
}

bool Process::freeResource(OS &sys) {
  sys.freeResource(this);
  return true;
}