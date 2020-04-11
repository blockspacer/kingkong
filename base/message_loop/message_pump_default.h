#ifndef _BASE_MESSAGE_PUMP_DEFALT_H_
#define _BASE_MESSAGE_PUMP_DEFALT_H_
#include "message_pump_impl.h"
#include <base_header.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

BEGIN_NAMESPACE_LOOPER

class MessagePumpDefatlt :public MessagePumpImpl {
public:
  MessagePumpDefatlt(const std::string& name, int32_t thread_count);
  ~MessagePumpDefatlt();

protected:
  void Wakeup(uint64_t expired_time) override;

  void DoRun() override;


  void DoStop() override;


private:
  boost::thread_group thread_;
  boost::asio::io_service io_service_;
  boost::asio::deadline_timer* timer_ = nullptr;
  // 增加一个work对象
  boost::asio::io_service::work* work_ = nullptr;
};

END_NAMESPACE_LOOPER
#endif
