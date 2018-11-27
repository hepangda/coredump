#include <thread>
#include <vector>
#include <random>
#include "os.h"
#include "process.h"

constexpr int kMaxThread = 64;
constexpr int kMaxAvailable = 200000;

void worker(OS *sys, int pid) {
	std::default_random_engine rd;

	uint need;
	do {
		need = rd() % kMaxAvailable;
	} while (need == 0);

	Process pro(need, 0, pid);

	while (pro.own_need() < pro.max_need()) {
		uint res = 0;

		do {
			res = (pro.max_need() - pro.own_need() > 1) ?
				rd() % (pro.max_need() - pro.own_need()) : 1;
		} while (res == 0);

		pro.requestResource(res, *sys);
	}

	printf("~~~[Thread] %u (%u) has request success, free resource.\n",
		pid, pro.max_need());
	pro.freeResource(*sys);
}

int main(int argc, char const *argv[]) {
  OS sys{};

  std::vector<std::thread> threads;
  threads.reserve(kMaxThread);

  for (int i = 0; i < kMaxThread; ++i) {
    threads.emplace_back(worker, &sys, i);
  }

  for (auto &i : threads) {
    i.join();
  }

  return 0;
}