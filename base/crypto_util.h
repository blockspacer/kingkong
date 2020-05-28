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

//计算MD5 hash
std::string Md5(const void* source, int32_t len);
std::string Md5(const std::string& source);


//RSA 加解密,公钥加密
bool EncryptByPublicKey(const void* buffer, int32_t len, std::string& result);
bool EncryptByPublicKey(const std::string& buffer, std::string& result);
bool RSA_Verify(const std::string& source, const std::string& sign);

#ifndef NDEBUG
bool DecryptByPrivateKey(const std::string& buffer, std::string& result);
bool DecryptByPrivateKey(const void* buffer, int len, std::string& result);
bool RSA_Sign(const std::string& source, std::string& sign);
#endif // NDEBUG


void HMAC_SHA256(const unsigned char *text, int text_len,
                 const unsigned char *key, int key_len, unsigned char *digest);


END_NAMESPACE_CRYPTO
#endif
