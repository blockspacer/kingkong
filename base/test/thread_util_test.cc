#include "thread_util.h"
#include "log.h"
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#ifdef ENABLE_THREAD_UTIL_TEST
BOOST_AUTO_TEST_CASE(ThreadUtil) {
  for (int i=0;i<10;i++) {
    std::string thread_id = BASE_THREAD::GetCurrentThreadId();
    LogDebug << thread_id;
    std::stringstream thread_name_stream;
    thread_name_stream << "ccustom:" << i;
    BASE_THREAD::SetCurrentThreadName(thread_name_stream.str());
  }
}
#endif
