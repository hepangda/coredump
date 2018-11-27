#ifndef BANKER_PROCESS_H
#define BANKER_PROCESS_H

using uint = unsigned int;

class OS;

class Process {
 public:
  Process(uint max, uint own, uint pid):
    max_need_(max), own_need_(own), pid_(pid), finish_(false) {}

  void set_max_need(uint val) { max_need_ = val; }
  void set_own_need(uint val) { own_need_ = val; }
  void set_pid(uint val) { pid_ = val; }
  void set_finish(bool val) { finish_ = val; }
  uint max_need() { return max_need_; }
  uint own_need() { return own_need_; }
  uint request_need() { return request_need_; }
  uint pid() { return pid_; }
  bool finish() { return finish_; }

  bool set_request_need(uint val) { 
    if (val <= max_need() - own_need()) {
      request_need_ = val;
      return true;
    }
    return false;
  }

  bool requestResource(uint num, OS &sys);
  bool freeResource(OS &sys);
 private:
  uint max_need_;
  uint own_need_;
  uint request_need_;
  uint pid_;

  bool finish_;
};

#endif // BANKER_PROCESS_H