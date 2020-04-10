#include "message_pump_default.h"
#include "semaphore_util.h"
#include "thread_util.h"
#include "time_util.h"
#include "log.h"
#include <boost/assert.hpp>
#include <boost/timer.hpp>
BEGIN_NAMESPACE_LOOPER
MessagePumpDefatlt::MessagePumpDefatlt(const std::string& name)
    : MessagePumpImpl(name) {
  auto semaphore = BASE_THREAD::CreateSigalSemaphore();
  thread_ = boost::thread([sem = semaphore.get(), this]{
    BASE_THREAD::SetCurrentThreadName(MessagePumpImpl::name());
    work_ = new boost::asio::io_service::work(io_service_);
    timer_ = new boost::asio::deadline_timer(io_service_);  
    sem->Signal();
    Run();
  });
  semaphore->Wait();
}


MessagePumpDefatlt::~MessagePumpDefatlt() {
  if (thread_.joinable()) {
    LogFatal << "thread is joinable";
  }
}

void MessagePumpDefatlt::Wakeup(uint64_t expired_time) {
  auto now = BASE_TIME::GetTickCount2();
  if (expired_time > now) {
    //使用定时器来异步等待
    timer_->expires_from_now(boost::posix_time::milliseconds(expired_time - now));
    timer_->async_wait([this] (boost::system::error_code) {
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
  io_service_.stop();
  if (thread_.joinable()) {
    thread_.join();
  }
  //线程停止之后，不会再操作work和定时器。所以可以不用加锁
  delete timer_;
  timer_ = nullptr;
  delete work_;
  work_ = nullptr;
}



END_NAMESPACE_LOOPER
