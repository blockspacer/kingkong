#include "message_pump_impl.h"
#include "message_pump_default.h"
#include "log.h"
#include "time_util.h"
#include <boost/random.hpp>
#include <boost/test/unit_test.hpp>


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
/*
BOOST_AUTO_TEST_CASE(MessagePump1) {
  auto pump = std::make_shared<BASE_LOOPER::MessagePumpDefatlt>("one");
  pump->PostRunable(
      [] { LogInfo << "test: 10  " << BASE_TIME::GetTickCount2(); }, 10000);

  pump->PostRunable(
      [] { LogInfo << "test: 5  " << BASE_TIME::GetTickCount2(); }, 5000);

  pump->PostRunable(
      [] { LogInfo << "test: 1  " << BASE_TIME::GetTickCount2(); }, 1000);

  boost::this_thread::sleep(boost::posix_time::seconds(100000));
}
*/
BOOST_AUTO_TEST_CASE(MessagePump) {
  std::vector<std::shared_ptr<BASE_LOOPER::MessagePumpDefatlt>> pumps;
  for (int i = 0; i < 10;i++) {
    std::stringstream thread_name_stream;
    thread_name_stream << "thread:" << i;
    pumps.push_back(std::make_shared<BASE_LOOPER::MessagePumpDefatlt>(thread_name_stream.str()));
  }
  for (auto& pump : pumps) {
    for (int j = 0; j < 1000; j++) {
      if (j % 2 == 0) {
        pump->PostRunable([] { LogInfo << boost::this_thread::get_id(); });
      } else {
        pump->PostRunable(
            [] {
              LogInfo << boost::this_thread::get_id()
                      << " ticks:" << BASE_TIME::GetTickCount2();
            },
            j * 1000);
      }
    }
  }
  boost::this_thread::sleep(boost::posix_time::seconds(10));
  boost::thread terminate_loop([=] {
    for (auto& pump : pumps) {
      pump->Stop();
    }
  });
  boost::this_thread::sleep(boost::posix_time::seconds(100000));
}
