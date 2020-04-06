#include "time_util.h"
#define BOOST_TEST_MODULE TIMEUTIL
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TIMEUTIL)

BOOST_AUTO_TEST_CASE(Timestamp) {
  uint64_t timestamp = BASE_TIME::GetTimestamp();
  printf("timestamp:%lld\n", timestamp);
  uint64_t timestamp_mesc = BASE_TIME::GetTimestampMsec();
  printf("timestamp_mesc:%lld\n", timestamp_mesc);
  std::tm now = BASE_TIME::MakeTime(timestamp);
  std::stringstream time_stream;
  time_stream << now.tm_year << "/"
    << now.tm_mon << "/"
    << now.tm_mday << " "
    << now.tm_hour << ":"
    << now.tm_min << ":"
    << now.tm_sec << "\n";
  printf (time_stream.str ().c_str ());

  for (size_t i = 0; i < 100; i++) {
    uint64_t tickcount = BASE_TIME::GetTickCount2();
    printf ("tickcount:%lld\n", tickcount);
  }
}

BOOST_AUTO_TEST_SUITE_END()