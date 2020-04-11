#include "message_pump_default.h"
#include "semaphore_util.h"
#include "thread_util.h"
#include "time_util.h"
#include "log.h"
#include <boost/assert.hpp>
#include <boost/timer.hpp>
BEGIN_NAMESPACE_LOOPER
//tls

void FreeNullTls(MessagePump*) {
}

boost::thread_specific_ptr<MessagePump> g_tls_(FreeNullTls);

MessagePumpDefatlt::MessagePumpDefatlt(const std::string& name, int32_t thread_count)
    : MessagePumpImpl(name) {
  auto semaphore = BASE_THREAD::CreateSigalSemaphore();
  for (int32_t i = 0; i < thread_count; i++) {
    thread_.create_thread([sem = semaphore.get(), this, i]{
      std::stringstream thread_name;
      thread_name << MessagePumpImpl::name() << ":" << (i + 1);
      BASE_THREAD::SetCurrentThreadName(thread_name.str());
      g_tls_.reset(this);
      if (nullptr == work_) {
        work_ = new boost::asio::io_service::work(io_service_);
      }
      if (nullptr == timer_) {
        timer_ = new boost::asio::deadline_timer(io_service_);
      }
      sem->Signal();
      Run();
    });
    semaphore->Wait();
  }
 
}


MessagePumpDefatlt::~MessagePumpDefatlt() {
  if (!stoped()) {
    LogFatal << "thread is joinable";
  }
}

void MessagePumpDefatlt::Wakeup(uint64_t expired_time) {
  auto now = BASE_TIME::GetTickCount2();
  if (expired_time > now) {
    //使用定时器来异步等待
    timer_->expires_from_now(boost::posix_time::milliseconds(expired_time - now));
    timer_->async_wait([this] (boost::system::error_code code) {
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
  io_service_.stop();
  thread_.join_all();
  //线程停止之后，不会再操作work和定时器。所以可以不用加锁
  delete timer_;
  timer_ = nullptr;
  delete work_;
  work_ = nullptr;
}


std::shared_ptr<MessagePump> MessagePumpDefatlt::CurrentPump() {
  if (g_tls_.get()) {
    return g_tls_->shared_from_this();
  }
  return nullptr;
}


END_NAMESPACE_LOOPER
