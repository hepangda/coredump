#include <list>
#include <unordered_map>

template <typename K, typename V, int LIMIT>
class LruCache {
 public:
  void set(K key, V value) {
    if (finder_.count(key) == 0) {
      if (values_.size() >= LIMIT) {
        finder_.erase(values_.back());
        values_.pop_back();
      }

      values_.emplace_front(value);
      finder_.emplace(key, values_.begin());
    } else {
      auto it = finder_[key];
      values_.emplace_front(*it);
      values_.erase(it);
      finder_[key] = values_.begin();
    }
  }

  typename std::list<V>::iterator get(K key) {
    auto t = finder_.find(key);
    if (t == finder_.end()) {
      return values_.end();
    }

    values_.emplace_front(*(t->second));
    values_.erase(finder_[key]);
    return finder_[key] = values_.begin();
  }

 private:
  std::list<V> values_;
  std::unordered_map<K, typename std::list<V>::iterator> finder_;
};