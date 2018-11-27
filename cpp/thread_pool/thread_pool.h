#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <memory>
#include <vector>
#include <functional>

#include "noncopyable.h"

namespace tg {

class ThreadPool : public Noncopyable {
 public:
  using Task = std::function<void()>;
  explicit ThreadPool(int num = std::thread::hardware_concurrency());
  ~ThreadPool() { stop(); }

  void stop();

  template<typename Callable, typename ...Args>
  void post(Callable &&func, Args &&...args) {
    std::lock_guard<std::mutex> auto_release(mutex_);
    task_queue_.emplace(std::bind(std::forward<Callable>(func), std::forward<Args>(args)...));
    not_empty_.notify_one();
  }

  int thread_num() const { return static_cast<int>(threads_.size()); }

  inline size_t task_size() const;

 private:
  std::vector<std::unique_ptr<std::thread>> threads_;
  std::atomic<bool> is_running_;

  mutable std::mutex mutex_;
  std::condition_variable not_empty_;
  std::queue<Task> task_queue_;

  std::shared_ptr<Task> take();
};

}
