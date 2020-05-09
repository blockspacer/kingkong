#define BOOST_TEST_MODULE Base
#include "log.h"
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_parameters.hpp>
#include <base/crypto_util.h>



BOOST_AUTO_TEST_CASE(Main) {
  //这里用来初始化一些全局配置
  std::string current_dir = boost::filesystem::current_path().string();
  BASE_LOG::InitLog(BASE_LOG::kLogLevelDebug, current_dir);


  for (int i=0;i<10;i++) {
    std::stringstream ss;
    for (int j=0;j<i;j++) {
      ss << "a";
    }
    std::string src = ss.str();
    std::string result = BASE_CRYPTO::Base64Encode(src);
    std::string decode = BASE_CRYPTO::Base64Decode(result);
    BOOST_TEST(src == decode);
  }

  for (size_t i = 0; i < 1000; i++) {
    std::stringstream ss;
    for (int j = 0; j < i; j++) {
      ss << "a";
    }
    std::string enc;
    BASE_CRYPTO::EncryptByPublicKey(ss.str(), enc);

    std::string dec;
    BASE_CRYPTO::DecryptByPrivateKey(enc, dec);
    BOOST_ASSERT(ss.str() == dec);

    std::string sign;
    BASE_CRYPTO::RSA_Sign(ss.str(), sign);

    BOOST_ASSERT(BASE_CRYPTO::RSA_Verify(ss.str(), sign));
  }
}
