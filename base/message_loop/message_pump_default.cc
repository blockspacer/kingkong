#include "message_pump_default.h"
#include "semaphore_util.h"
#include "thread_util.h"
#include "time_util.h"
#include "log.h"
#include <boost/assert.hpp>
#include <boost/timer.hpp>
BEGIN_NAMESPACE_LOOPER

MessagePumpDefatlt::MessagePumpDefatlt(const std::string& name, int32_t thread_count)
    : MessagePumpImpl(name),
  timer_(io_service_),
  work_(io_service_) {
  auto semaphore = BASE_THREAD::CreateSigalSemaphore();
  for (int32_t i = 0; i < thread_count; i++) {
    thread_.create_thread([sem = semaphore.get(), this, i]{
      std::stringstream thread_name;
      thread_name << MessagePumpImpl::name() << ":" << (i + 1);
      BASE_THREAD::SetCurrentThreadName(thread_name.str());
      BASE_THREAD::SetThreadTls(this);     
      sem->Signal();
      Run();
    });
    semaphore->Wait();
  }
 
}


MessagePumpDefatlt::~MessagePumpDefatlt() {
}

void MessagePumpDefatlt::Wakeup(uint64_t expired_time) {
  auto now = BASE_TIME::GetTickCount2();
  if (expired_time > now) {
    //使用定时器来异步等待
    timer_.expires_from_now(boost::posix_time::milliseconds(expired_time - now));
    timer_.async_wait([this] (boost::system::error_code code) {
      //定时器被取消的时候，也会收到回调，这里需要判断一下
      if (code == boost::asio::error::operation_aborted) {
        return;
      }
      DoWork();
    });
  } else {
    //已经到期了，直接抛一个异步任务
    io_service_.post([this] {
      DoWork();
    });
  }
}

void MessagePumpDefatlt::DoRun() {
  io_service_.run();
}

void MessagePumpDefatlt::DoStop() {
  boost::system::error_code ec;
  timer_.cancel(ec);
  io_service_.stop();
}

void* MessagePumpDefatlt::Raw() {
  return &io_service_;
}

void MessagePumpDefatlt::Join() {
  thread_.join_all();
}

END_NAMESPACE_LOOPER



