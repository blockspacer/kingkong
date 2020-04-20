#include "message_loop/message_loop.h"
#include "log.h"
#include "time_util.h"
#include "message_loop/message_pump_impl.h"
#include "message_loop/message_pump_default.h"
#include <boost/random.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/random.hpp>

#include <boost/random/random_device.hpp>


#ifdef MESSAGE_PUMP_TEST

BOOST_AUTO_TEST_CASE(RunnableInfo) {
  boost::variate_generator<boost::mt19937, boost::uniform_int<> > vgen(boost::mt19937(), boost::uniform_int<>(0, 999));
  std::set< BASE_LOOPER::RunnableInfo> runnable_set;
  for (int32_t i=0;i<990;i++)  {
    runnable_set.insert(BASE_LOOPER::RunnableInfo(vgen(), i, nullptr));
  }
  uint64_t last_expired_time = 0;
  for (auto& item : runnable_set) {
    uint64_t next_expired_time = item.expired_time;
    BOOST_TEST(next_expired_time >= last_expired_time);
    last_expired_time = next_expired_time;
  }
}

BOOST_AUTO_TEST_CASE(MessagePump) {
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  boost::uniform_int<> real(2, 10);
  boost::random::mt19937 gen;
  auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();

  for (int i = 0; i < 10000; i++) {
    io_pump->PostRunable([i] {
      LogInfo << "index:" << i;
      });
  }
      boost::this_thread::sleep(boost::posix_time::seconds(5));
    BASE_LOOPER::MessageLoop::UnintMessageLoop();
}
#endif