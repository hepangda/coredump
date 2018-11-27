#include <unistd.h>
#include <sys/sem.h>
#include <semaphore.h>

#include <iostream>
#include <thread>
#include <future>
#include <queue>
#include <random>

using Product = int;
constexpr int kBufferSize = 20;

class Buffer : public std::queue<int> {
 public:
  inline Product take() {
    Product ret = front();
    pop();
    return ret;
  }

  inline void put(Product val) {
    push(val);
  }
};

class TSemaphore {
 public:
  TSemaphore(unsigned int val = 0) {
    sem_init(&origin_sem_, 0, val);
  }
  
  int wait() {
    return sem_wait(&origin_sem_);
  }

  int post() {
    return sem_post(&origin_sem_);
  }

  TSemaphore(const TSemaphore &) = delete;
  TSemaphore &operator =(const TSemaphore &) = delete;
 private:
  sem_t origin_sem_;
};

Buffer buf;
TSemaphore mutex(1), filled(0), empty(kBufferSize);

void producer()
{
  std::default_random_engine produce_machine; 

  for (;;) {
    Product n = produce_machine();

    empty.wait();
    mutex.wait();

    buf.put(n);

    mutex.post();
    filled.post();    
  }
}

void consumer()
{
  auto consume = [] (const Product &prod) {
    std::cout << prod << std::endl;
    usleep(100000);
  };

  for (;;) {
    filled.wait();
    mutex.wait();

    Product n = buf.take();

    mutex.post();
    empty.post();

    consume(n);
  }
}

int main(int argc, char *argv[])
{
  auto produce_thread = std::async(std::launch::async, producer),
       consume_thread = std::async(std::launch::async, consumer);
}