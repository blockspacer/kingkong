#include "message_pump_impl.h"
#include "time_util.h"
#include "log.h"
#include <boost/assert.hpp>
BEGIN_NAMESPACE_LOOPER

MessagePumpImpl::MessagePumpImpl(const std::string& name) : name_(name) {}

 MessagePumpImpl::~MessagePumpImpl() {
  if (!stoped()) {
    LogFatal << "message pump is not stoped:" << name_;
  }
 }

const char* MessagePumpImpl::name() {
  return name_.c_str();
}

void MessagePumpImpl::Run() {
  DoRun();
  stoped_ = true;
  std::lock_guard<std::mutex> lock(runable_set_mutex_);
  runable_set_.clear();
}

void MessagePumpImpl::Stop() {
  if (stoped_) {
    return;
  }
  DoStop();
  stoped_ = true;
}

uint64_t MessagePumpImpl::PostRunable(Runnable runable) {
  return PostRunable(std::move(runable), 0);
}

uint64_t MessagePumpImpl::PostRunable(Runnable runnable, uint64_t delay) {
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

bool MessagePumpImpl::Cancel(uint64_t id) {
  std::lock_guard<std::mutex> lock(runable_set_mutex_);
  for (auto iter = runable_set_.begin(); iter != runable_set_.end(); iter++) {
    if (iter->id == id) {
      runable_set_.erase(iter);
      return true;
    }
  }
  return false;
}

void MessagePumpImpl::DoOneWork() {
  if (stoped_) {
    return;
  }
  uint64_t now = BASE_TIME::GetTickCount2();
  Runnable runnable;
  {
    //找到一个到期的任务
     std::lock_guard<std::mutex> lock(runable_set_mutex_);
    if (!runable_set_.empty() && (runable_set_.begin()->expired_time <= now)) {
      runnable = std::move(runable_set_.begin()->runnable);
      runable_set_.erase(runable_set_.begin());
    }
    //先唤醒，然后执行runnalbe。否则可能 runnable domodel 导致阻塞
    if (!runable_set_.empty()) {
      Wakeup(runable_set_.begin()->expired_time);
    }
  }
  if (runnable) {
    runnable();
  }
}
void MessagePumpImpl::DoWork() {
  if (stoped_) {
    return;
  }
  uint64_t now = BASE_TIME::GetTickCount2();
  std::vector<Runnable> runnables;
  {
    //set 中是按时间顺序排列，取到不符合的直接break就可以
    std::lock_guard<std::mutex> lock(runable_set_mutex_);
    for (auto begin = runable_set_.begin(); begin != runable_set_.end();) {
      if (begin->expired_time <= now) {
        runnables.push_back(std::move(begin->runnable));
        begin = runable_set_.erase(begin);
      } else {
        break;
      }
    }
  }
  for (auto& runable : runnables) {
    runable();
  }

  {
    std::lock_guard<std::mutex> lock(runable_set_mutex_);
    if (!runable_set_.empty()) {
      Wakeup(runable_set_.begin()->expired_time);
    }
  }
}
END_NAMESPACE_LOOPER


