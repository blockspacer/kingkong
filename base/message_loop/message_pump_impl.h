#ifndef _BASE_MESSAGE_PUMP_IMPL_H_
#define _BASE_MESSAGE_PUMP_IMPL_H_
#include "message_pump.h"
#include <base_header.h>
#include <atomic>
#include <boost/utility/string_view.hpp>

BEGIN_NAMESPACE_LOOPER

struct RunnableInfo {
  RunnableInfo(uint64_t expired_time, uint64_t id, Runnable runnable) {
    this->expired_time = expired_time;
    this->id = id;
    this->runnable = std::move(runnable);
  }

  RunnableInfo(RunnableInfo&& other) {
    expired_time = other.expired_time;
    id = other.id;
    runnable = std::move(other.runnable);
  }

  RunnableInfo& operator=(RunnableInfo&& other) {
    expired_time = other.expired_time;
    id = other.id;
    runnable = std::move(other.runnable);
    return *this;
  }

  bool operator < (const RunnableInfo& other) const {
    if (expired_time == other.expired_time) {
      return id < other.id;
    }
    return expired_time < other.expired_time;
  }
  uint64_t expired_time;
  Runnable  runnable;
  uint64_t id;
};

class MessagePumpImpl :public MessagePump {
public:
  explicit MessagePumpImpl(const std::string& name);
  ~MessagePumpImpl();

  const char* name() override;

  uint64_t PostRunable(Runnable runnable) override;

  uint64_t PostRunable(Runnable runnable, uint64_t delay) override;

  bool Cancel(uint64_t id) override;

  void Stop() override;

  bool stoped() {
    return stoped_;
  }

  virtual void Wakeup(uint64_t expired_time) = 0;
  virtual void DoRun() = 0;
  virtual void DoStop() = 0;

protected:
  void Run() override;

  //在当前loop线程执行，取出所有到期的任务
  void DoWork();

  //对于部分会阻塞消息循环的任务，需要一个一个执行。 比如windows 的模态框
  void DoOneWork();

private:
  std::set<RunnableInfo> runable_set_;
  std::mutex runable_set_mutex_;
  std::string name_;
  uint64_t id_ = 0;
  std::atomic<bool> stoped_;
};

END_NAMESPACE_LOOPER
#endif
