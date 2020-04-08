#include "looper_impl.h"
#include "time_util.h"
#include <boost/assert.hpp>
BEGIN_NAMESPACE_LOOPER

LooperImpl::LooperImpl(const boost::string_view& name) : name_(name) {}

const char* LooperImpl::name() {
  return name_.c_str();
}

uint64_t LooperImpl::PostRunable(Runnable runable) {
  return PostRunable(std::move(runable), 0);
}

uint64_t LooperImpl::PostRunable(Runnable runnable, uint64_t delay) {
  if (stoped_) {
    return 0;
  }
  uint64_t expired_time = BASE_TIME::GetTickCount2() + delay;
  uint64_t id = 0;
  bool wakeup = false;
  {
    std::lock_guard<std::mutex> lock(runable_set_mutex_);
    id = ++id_;
    if (runable_set_.empty() ||
        (expired_time < runable_set_.begin()->expired_time)) {
      wakeup = true;
    }
    RunnableInfo runable_info(expired_time, id, std::move(runnable));
    runable_set_.insert(std::move(runable_info));
  }
  if (wakeup) {
    Wakeup(expired_time);
  }
  return id;
}

bool LooperImpl::Cancel(uint64_t id) {
  std::lock_guard<std::mutex> lock(runable_set_mutex_);
  for (auto iter = runable_set_.begin(); iter != runable_set_.end(); iter++) {
    if (iter->id == id) {
      runable_set_.erase(iter);
      return true;
    }
  }
  return false;
}

END_NAMESPACE_LOOPER


