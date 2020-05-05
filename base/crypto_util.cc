#include "crypto_util.h"
#include <openssl/base64.h>

BEGIN_NAMESPACE_CRYPTO

std::string Base64Encode(const void* buffer, int32_t len) {
  size_t max_encode_len = 0;
  EVP_EncodedLength(&max_encode_len, len);
  std::string result(max_encode_len, 0);
  size_t result_len = EVP_EncodeBlock((uint8_t*)(&result[0]), (uint8_t*)buffer, len);
  result.resize(result_len);
  return result;
}

std::string Base64Encode(const std::string& buffer) {
  return Base64Encode(buffer.c_str(), buffer.length());
}

std::string Base64Decode(const void* buffer, int32_t len) {
  size_t max_decode_len = 0;
  EVP_DecodedLength(&max_decode_len, len);
  std::string result(max_decode_len, 0);
  size_t result_len = 0;
  if (EVP_DecodeBase64((uint8_t*)(&result[0]), &result_len, max_decode_len,
                       (uint8_t*)buffer, len)) {
    result.resize(result_len);
    return result;
  }
  return "";
}

std::string Base64Decode(const std::string& buffer) {
  return Base64Decode(buffer.c_str(), buffer.length());
}

END_NAMESPACE_CRYPTO


