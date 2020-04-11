#include "message_loop/message_loop.h"
#include "log.h"
#include "time_util.h"
#include "message_loop/message_pump_impl.h"
#include "message_loop/message_pump_default.h"
#include <boost/random.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/random.hpp>

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
BOOST_AUTO_TEST_CASE(MessagePumpIO) {
  auto io_pump =
      std::make_shared<BASE_LOOPER::MessagePumpDefatlt>("io", 1);
  io_pump->PostRunable(
      [] { boost::this_thread::sleep(boost::posix_time::seconds(50000)); }, 100000);

  io_pump->PostRunable(
      [] { boost::this_thread::sleep(boost::posix_time::seconds(50000)); },
      10000);
  boost::this_thread::sleep(boost::posix_time::seconds(1000));
}


BOOST_AUTO_TEST_CASE(MessagePumpWork) {
  auto work_pump = std::make_shared<BASE_LOOPER::MessagePumpDefatlt>("work", 10);
  work_pump->PostRunable([] {
    boost::this_thread::sleep(boost::posix_time::seconds(5000));
    });
  boost::uniform_int<> real(2, 10);
  boost::random::mt19937 gen;
  for (int i=0;i<100;i++) {
    work_pump->PostRunable([] {
      LogInfo << "loopname: "
              << BASE_LOOPER::MessageLoop::CurrentMessagePump()->name();
      });

    work_pump->PostRunable([] {
      LogInfo << "loopname: "
              << BASE_LOOPER::MessageLoop::CurrentMessagePump()->name();
    }, real(gen) * 1000);
  }

  //boost::this_thread::sleep(boost::posix_time::seconds(10));
  work_pump->Stop();
}*/

BOOST_AUTO_TEST_CASE(MessagePump) {
  while (true) {
    BASE_LOOPER::MessageLoop::InitMessageLoop();
      boost::uniform_int<> real(2, 10);
      boost::random::mt19937 gen;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      auto work_pump = BASE_LOOPER::MessageLoop::WorkMessagePump();
      auto file_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (size_t i = 0; i < 1000; i++) {
        io_pump->PostRunable([] {
          LogInfo << "loopname: "
                  << BASE_LOOPER::MessageLoop::CurrentMessagePump()->name();         
        });

        io_pump->PostRunable(
            [] {
            LogInfo << "loopname: "
                      << BASE_LOOPER::MessageLoop::CurrentMessagePump()->name();
            },
            real(gen) * 1000);

        work_pump->PostRunable([] {
          LogInfo << "loopname: "
                  << BASE_LOOPER::MessageLoop::CurrentMessagePump()->name();

        });

        work_pump->PostRunable(
            [] {
             LogInfo << "loopname: "
                      << BASE_LOOPER::MessageLoop::CurrentMessagePump()->name();
              boost::uniform_int<> real3(2, 10);
              boost::random::mt19937 gen3;
              boost::this_thread::sleep(
                  boost::posix_time::seconds(real3(gen3)));

            },
            real(gen) * 1000);

        file_pump->PostRunable([] {
          LogInfo << "loopname: "
                  << BASE_LOOPER::MessageLoop::CurrentMessagePump()->name();
          });

        file_pump->PostRunable(
          [] {
            LogInfo << "loopname: "
                     << BASE_LOOPER::MessageLoop::CurrentMessagePump()->name();
          },
          real(gen) * 1000);
      }
    boost::uniform_int<> real2(2, 10);
    boost::random::mt19937 gen2;
    boost::this_thread::sleep(boost::posix_time::seconds(real2(gen2)));
    BASE_LOOPER::MessageLoop::UnintMessageLoop();
  }
}
