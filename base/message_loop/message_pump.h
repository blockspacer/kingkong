#ifndef _BASE_MESSAGE_PUMP_H_
#define _BASE_MESSAGE_PUMP_H_
#include <base/base_header.h>
BEGIN_NAMESPACE_LOOPER

using Runnable = std::function<void()>;

class MessagePump : public std::enable_shared_from_this<MessagePump> {
 public:
  virtual ~MessagePump() = default;
  //线程名
  virtual const char* name() = 0;
  //启动Looper
  virtual void Run() = 0;
  //结束Looper
  virtual void Stop() = 0;
  //向Looper 抛一个runable， 在当前的looper 线程中执行
  virtual uint64_t PostRunable(Runnable runable) = 0;
  //向Looper 抛一个runable， 在当前的looper 线程中延迟执行， delay 单位秒
  virtual uint64_t PostRunable(Runnable runable, uint64_t delay) = 0;
  //取消一个Runable
  virtual bool     Cancel(uint64_t id) = 0;
  //获取io_service 指针
  virtual void* Raw() {
    return nullptr;
  }
};

END_NAMESPACE_LOOPER
#endif
