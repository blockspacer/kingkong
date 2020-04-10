#define BOOST_TEST_MODULE Base
#include "log.h"
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_parameters.hpp>

BOOST_AUTO_TEST_CASE(Main) {
  //这里用来初始化一些全局配置
  std::string current_dir = boost::filesystem::current_path().string();
  BASE_LOG::InitLog(BASE_LOG::kLogLevelDebug, current_dir);
}
