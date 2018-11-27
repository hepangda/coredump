class Noncopyable {
 public:
  // delete copy constructor and copy assign operator to avoid copy
  Noncopyable(const Noncopyable &) = delete;
  Noncopyable &operator=(const Noncopyable &) = delete;
 protected:
  // declared `protected` to avoid get instance of `Noncopyable` itself
  constexpr Noncopyable() = default;
  ~Noncopyable() = default;
};
