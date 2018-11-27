#include <cstdio>

#include "os.h"
#include "process.h"

void OS::printList() {
  for (auto &i : process_id_) {
    printf("%u ", i);
  }
  putchar('\n');
}

uint OS::attainResource(Process *process) {
  std::lock_guard<std::mutex> auto_release(mutex_);

  uint request_num = process->request_need();
  uint available_bk = available();
  uint own_need_bk = process->own_need();

  if (request_num <= available()) {
    set_available(available() - request_num);
    process->set_own_need(process->own_need() + request_num);

    printf("\n[System] %u\n", available_bk);

    if (banker(*process)) {
      process_list_.insert(process);

      printf("[Thread] %u\t%u\t%u\t%u\tSUCCEES\n", process->pid(),
             process->max_need(), process->own_need(), process->request_need());

      printList();
      return 0;
    } else {
      set_available(available_bk);
      process->set_own_need(own_need_bk);

      printf("[Thread] %u\t%u\t%u\t%u\tFAILED\n", process->pid(),
             process->max_need(), process->own_need(), process->request_need());

      printList();
      return 1;
    }
  }

  return 1;
}

bool OS::banker(Process &process) {
  uint available_bk = available();
  uint num = process.request_need();

  process_id_.clear();

  for (auto i = process_list_.begin(); i != process_list_.end(); ) {
    if ((*i)->finish() == false && (*i)->max_need() - (*i)->own_need() <= available()) {
      (*i)->set_finish(true);
      set_available(available() + (*i)->own_need());
      process_id_.push_back((*i)->pid());
      i = process_list_.begin();
    } else {
      ++i;
    }
  }

  bool ret = true;
  for (auto &i: process_list_) {
    if (!i->finish()) ret = false;
    i->set_finish(false);
  }

  set_available(available_bk);
  return ret;
}

bool OS::freeResource(Process *process) {
  std::lock_guard<std::mutex> auto_release(mutex_);

  set_available(available() + process->own_need());
  process_list_.erase(process);

  return process_list_.find(process) != process_list_.end();
}