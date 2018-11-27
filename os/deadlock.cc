#include <cstdio>
#include <thread>
#include <mutex>

constexpr int kLoopTimes = 10000;
std::mutex mutex1, mutex2;

void critical_section(int thread_num, int i);

int main(int argc, char *argv[]) {
  std::thread child { [] {
    for (int i = 0; i < kLoopTimes; ++i) {
      mutex2.lock();
      mutex1.lock();

      critical_section(2, i);

      mutex2.unlock();
      mutex1.unlock();
    }
  } };


  for (int i = 0; i < kLoopTimes; ++i) {
    mutex1.lock();
    mutex2.lock();

    critical_section(1, i);

    mutex2.unlock();
    mutex1.unlock();
  }
}

void critical_section(int thread_num, int i) {
  // printf("Thread%d:%d\n", thread_num, i);
}