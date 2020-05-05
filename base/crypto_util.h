#ifndef _CRYPTO_UTIL_H_
#define _CRYPTO_UTIL_H_
#include <base/base_header.h>

BEGIN_NAMESPACE_CRYPTO
//base64 编码
std::string Base64Encode(const void* buffer, int32_t len);
std::string Base64Encode(const std::string& buffer);

//base64 解码
std::string Base64Decode(const void* buffer, int32_t len);
std::string Base64Decode(const std::string& buffer);

END_NAMESPACE_CRYPTO
#endif
