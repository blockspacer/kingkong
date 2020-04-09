#include "log.h"
#include <boost/random.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/test/unit_test.hpp>
#ifdef ENABLE_LOG_TEST

BOOST_AUTO_TEST_CASE(Log) {
  std::string current_dir = boost::filesystem::current_path().string();
  BASE_LOG::InitLog(BASE_LOG::kLogLevelDebug, current_dir);
  boost::thread_group group;
  for (int i = 0; i < 100; i++) {
    group.create_thread([i] {
      for (int j = 0; j < 100000; j++) {
        boost::this_thread::sleep(boost::posix_time::seconds(5));
        switch ((i + j) % 5) {
          case 0: {
            LogTrace << "test log:" << i << ":" << j << true;
          } break;
          case 1: {
            LogDebug << "test log:" << i << ":" << j << true;
          } break;
          case 2: {
            LogInfo << "test log:" << i << ":" << j << true;
          } break;
          case 3: {
            LogWarning << "test log:" << i << ":" << j << true;
          } break;
          case 4: {
            LogError << "test log:" << i << ":" << j << true;
          } break;
          case 5: {
            LogFatal << "test log:" << i << ":" << j << true;
          } break;
        }
      }
    });
  }
  group.join_all();
}

#endif // ENABLE_LOG_TEST
