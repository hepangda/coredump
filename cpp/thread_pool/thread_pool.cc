#include "thread_pool.h"

namespace tg {

ThreadPool::ThreadPool(int num) : is_running_(true) {
  for (int i = 0; i < num; ++i) {
    threads_.emplace_back(std::make_unique<std::thread>([this] {
      while (is_running_.load()) {
        auto task = take();
        (*task)();
      }
    }));
  }
}

size_t ThreadPool::task_size() const {
  std::lock_guard<std::mutex> auto_release(mutex_);
  return task_queue_.size();
}

std::shared_ptr<ThreadPool::Task> ThreadPool::take() {
  std::unique_lock<std::mutex> auto_release(mutex_);

  // loop to avoid wakeup when other threads have pop()
  // before this thread finish pop().
  while (task_queue_.empty()) {
    not_empty_.wait(auto_release);
  }

  if (is_running_.load()) {
    auto task = std::make_shared<ThreadPool::Task>(task_queue_.front());
    task_queue_.pop();
    return std::move(task);
  } else {
    return nullptr;
  }
}

void ThreadPool::stop() {
  if (!is_running_.load()) {
    not_empty_.notify_all();
    is_running_.store(false);
    for (auto &i: threads_) {
      i->join();
    }
  }
}

}