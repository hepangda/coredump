#include <iostream>
#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <cassert>

class Noncopyable {
 public:
  // delete copy constructor and copy assign operator to avoid copy
  Noncopyable(const Noncopyable &) = delete;
  Noncopyable &operator=(const Noncopyable &) = delete;

 protected:
  // ｄeclared `protected` to avoid get instance of `Noncopyable` itself
  constexpr Noncopyable() = default;
  ~Noncopyable() = default;
};

template<typename T>
class BlockQueue : public Noncopyable {
 public:
  void push(const T &x) {
    std::lock_guard<std::mutex> auto_release(mutex_);
    queue_.emplace_back(x);
    notEmpty_.notify_all(); // be the same as notify_one()
  }

  void push(T &&x) {
    std::lock_guard<std::mutex> auto_release(mutex_);
    queue_.emplace_back(x);
    notEmpty_.notify_all();
  }

  T pop() {
    std::unique_lock<std::mutex> auto_release(mutex_);

    // loop to avoid wakeup when other threads have pop()
    // before this thread finish pop().
    while (queue_.empty()) {
      notEmpty_.wait(auto_release);
    }

    assert(!queue_.empty());

    // use move constructor for high performance
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return std::move(front);
  }

  size_t size() const {
    std::lock_guard<std::mutex> auto_release(mutex_);
    return queue_.size();
  }
 private:
  mutable std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::deque<T> queue_;
};

#include <unistd.h>
using namespace std;

int main()
{
  BlockQueue<int> Q;

  thread t1([&Q] {
    sleep(1);
    for (int i = 0; i < 3000 ; i++) {
      Q.push(i);
    }
  });

  thread t2([&Q] {
    for (;;) {
      int t = std::move(Q.pop());
      cout << t << endl;
    }
  });

  t1.join();
  t2.join();
}