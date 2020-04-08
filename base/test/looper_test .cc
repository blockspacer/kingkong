#include "looper_impl.h"
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
