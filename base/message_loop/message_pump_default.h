#ifndef _BASE_MESSAGE_PUMP_DEFALT_H_
#define _BASE_MESSAGE_PUMP_DEFALT_H_
#include <base/message_loop/message_pump_impl.h>
#include <base/base_header.h>
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

  void* Raw() override;

  void Join() override;

private:
  boost::thread_group thread_;
  boost::asio::io_service io_service_;
  boost::asio::deadline_timer timer_;
  // 增加一个work对象
  std::unique_ptr<boost::asio::io_service::work> work_;
};

END_NAMESPACE_LOOPER
#endif
