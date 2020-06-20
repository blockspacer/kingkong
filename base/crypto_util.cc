#include "crypto_util.h"
#include <openssl/base64.h>
#include <openssl/md5.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <boost/assert.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include "string_util.h"

BEGIN_NAMESPACE_CRYPTO

char* g_pszPubKey = "-----BEGIN PUBLIC KEY-----\n"
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC3uYMp/+p71xi1B0e5Nkfhps8lonVoi81081uA\n"
"3Ep1rcDbTby17WyMPkfoqcypMBDezBVUCSqD4z2Y8CsAoWyqm0VMzymtGbWguBnoQ3Bg0o0NVu4O\n"
"TsR+pitRAQTMVS3m5XhdgLU3lXHdGIOcsmfrwCpk8fZ9FZD44BPY+1CHBwIDAQAB\n"
"-----END PUBLIC KEY-----\n";

#ifndef NDEBUG
char* g_pszPrivateKey = "-----BEGIN PRIVATE KEY-----\n"
"MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBALe5gyn/6nvXGLUHR7k2R+GmzyWi\n"
"dWiLzXTzW4DcSnWtwNtNvLXtbIw+R+ipzKkwEN7MFVQJKoPjPZjwKwChbKqbRUzPKa0ZtaC4GehD\n"
"cGDSjQ1W7g5OxH6mK1EBBMxVLebleF2AtTeVcd0Yg5yyZ+vAKmTx9n0VkPjgE9j7UIcHAgMBAAEC\n"
"gYEAhMmREl8EE+O/2L9dSHEB/Qa6OyIzJ4+7vGZTYUxWiL0/YYHK/daPGMG+6VsoL7QszUrFs4o4\n"
"b0BjedFD013YkjDGKCrxcAaNCQOVG42R6MuaLneZEXytSS50yKiGXeVZOBhQrDeh0UGxs7m94/WO\n"
"bKxf3Hh9oernIYXvNZgQHzECQQDcCiumnLgALIGaa8A/u8X5rmVU2w2D52vxD+O93+9xqBf6RB7F\n"
"T0tvkp7pWz0CSnlrymCxtmaU/q7hVj3b3mrfAkEA1cAGEemwmK8fqfNnoOE3yeu1QYhACz6VcN0E\n"
"6fOehAcvz0Y6jBYFdzuz4L/v0MoJO5IIG6OUP5Hp8EurKCYQ2QJAfTsgmsOpCRhLbsqScPN6el7h\n"
"9whwybiFO35SnHb9vM7xTpNEfAFkKozk+mTd9RbTwiA9JgdsnLHScBb4P9F3OwJAVLOdNCf4lOiA\n"
"kaYkoHne06MV1nwZdNs9+NV8hwDFBeibJ3Xa6XwoelEsPJmSfJgzBGof0GQH8RDvNQunLWe4MQJA\n"
"RZFi6J1BWlV3UHBVLQgB1V5kyKIzFDHmc1lg7Jr/S838QaBQc/kxho1mi3LoBJZaS2LIVNrChnt9\n"
"8lT/RcKuvg==\n"
"-----END PRIVATE KEY-----\n";
#endif

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

std::string Base64EncodeUrlSafe(const void* buffer, int32_t len) {
  std::string base_64 = Base64Encode(buffer, len);
  boost::algorithm::replace_all(base_64, "+", "-");
  boost::algorithm::replace_all(base_64, "/", "_");
  boost::algorithm::replace_all(base_64, "=", "");
  return base_64;
}

std::string Base64EncodeUrlSafe(const std::string& buffer) {
  return Base64EncodeUrlSafe(buffer.c_str(), buffer.length());
}

std::string Md5(const std::string& source) {
  char digst[16] = { 0 };
  MD5((const uint8_t*)source.c_str(), source.length(),(uint8_t*)digst);
  return std::string(digst, 16);
}

std::string Md5(const void* source, int32_t len) {
  char digst[16] = { 0 };
  MD5((const uint8_t*)source, len, (uint8_t*)digst);
  return std::string(digst, 16);
}

std::string Md5Hex(const void *source, int32_t len) {
  return BASE_STRING_UTIL::BinToHex(Md5(source, len));
}

std::string Md5Hex(const std::string &source) {
  return Md5Hex(source.c_str(), source.length());
}

bool EncryptByPublicKey(const void* buffer, int32_t len, std::string& result) {
  if (0 == len) {
    return false;
  }
  BIO* bio = NULL;
  if ((bio = BIO_new_mem_buf(g_pszPubKey, -1)) == NULL)       //从字符串读取RSA公钥
  {
    return false;
  }
  RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);   //从bio结构中得到rsa结构
  if (nullptr == rsa)
  {
    BIO_free_all(bio);
    return false;
  }
  //加密之后的密文长度和密钥相同, 100 字节分为一段
  int enc_result_len = ((len - 1) / 100 + 1) * RSA_size(rsa);
  result.resize(enc_result_len);
  //开始分段了
  int already_enc_len = 0; //已经加密的明文长度
  // 已经加密的密文长度
  int enc_data_len = 0;
  while (already_enc_len < len)
  {
    int current_enc_len = (std::min)(100, len - already_enc_len);

    int ret = RSA_public_encrypt(current_enc_len, (const uint8_t*)buffer + already_enc_len, (unsigned char*)&result[0] + enc_data_len, rsa, RSA_PKCS1_PADDING /*RSA_NO_PADDING*/);


    enc_data_len += ret;
    already_enc_len += current_enc_len;
  }

  BIO_free_all(bio);
  RSA_free(rsa);
  assert(enc_data_len == enc_result_len);
  return enc_data_len == enc_result_len;
}

bool EncryptByPublicKey(const std::string& buffer, std::string& result) {
  return EncryptByPublicKey(buffer.c_str(), buffer.length(), result);
}

bool RSA_Verify(const std::string& source, const std::string& sign) {
  BIO* bio = NULL;
  if ((bio = BIO_new_mem_buf(g_pszPubKey, -1)) == NULL) {    //从字符串读取RSA公钥
    return false;
  }
  RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);   //从bio结构中得到rsa结构
  if (nullptr == rsa)
  {
    BIO_free_all(bio);
    return false;
  }

  std::string md5 =  Md5(source);
  int ret = RSA_verify(NID_md5, (const uint8_t*)md5.c_str(), md5.length(), (const uint8_t*)sign.c_str(), sign.length(), rsa);
  BIO_free_all(bio);
  RSA_free(rsa);
  return (ret == 1);
}

#ifndef NDEBUG
bool DecryptByPrivateKey(const void* buffer, int len, std::string& result) {
  BIO* bio = NULL;
  if ((bio = BIO_new_mem_buf(g_pszPrivateKey, -1)) == NULL)       //从字符串读取RSA公钥
  {
    return false;
  }
  RSA* rsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);   //从bio结构中得到rsa结构
  if (nullptr == rsa)
  {
    BIO_free_all(bio);
    return false;
  }
  int key_bytes = RSA_size(rsa);
  assert(len % key_bytes == 0);
  result.resize(len);    //开始分段了
  int decrypted_len = 0; //已经解密的密文长度
  // 已经加密的密文长度
  int result_data_len = 0;
  while (decrypted_len < len)
  {
    int ret = RSA_private_decrypt(key_bytes, (const uint8_t*)buffer + decrypted_len, (uint8_t*)&result[0] + result_data_len, rsa, RSA_PKCS1_PADDING);
    if (-1 == ret)
    {
      break;
    }
    result_data_len += ret;
    decrypted_len += key_bytes;
  }
  BIO_free_all(bio);
  RSA_free(rsa);
  result.resize(result_data_len);
  return decrypted_len == len;
}

bool DecryptByPrivateKey(const std::string& buffer, std::string& result) {
  return DecryptByPrivateKey(buffer.c_str(), buffer.length(), result);
}

bool RSA_Sign(const std::string& source, std::string& sign) {
  BIO* bio = NULL;
  if ((bio = BIO_new_mem_buf(g_pszPrivateKey, -1)) == NULL)       //从字符串读取RSA公钥
  {
    return false;
  }
  RSA* rsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);   //从bio结构中得到rsa结构
  if (nullptr == rsa)
  {
    BIO_free_all(bio);
    return false;
  }
  sign.resize(RSA_size(rsa));

  std::string md5 = Md5(source);

  unsigned int sign_len = 0;
  int iRet = RSA_sign(NID_md5, (const uint8_t*)md5.c_str(), md5.length(), (uint8_t*)&sign[0], &sign_len, rsa);
  BIO_free_all(bio);
  RSA_free(rsa);
  return iRet == 1;
}

std::string HMAC_SHA256(const unsigned char* text, int text_len,
                 const unsigned char* key, int key_len) {
  std::string resutl;
#if OPENSSL_VERSION_NUMBER >= 0x1010000fL
  HMAC_CTX* ctx = HMAC_CTX_new();
  if (!ctx) {
    return "";
  }
#else
  HMAC_CTX* ctx = (HMAC_CTX*)malloc(sizeof(HMAC_CTX));
  if (!ctx) {
    return "";
  }
  HMAC_CTX_init(ctx);
#endif
  do {
    if (HMAC_Init_ex(ctx, key, key_len, EVP_sha256(), 0) != 1) {
      break;
    }
    HMAC_Update(ctx, text, text_len);

    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int len = 0;
    HMAC_Final(ctx, md, &len);
    BOOST_VERIFY(len == 32);
    resutl.resize(len);
    memcpy(&resutl[0], md, len);
  } while (false);

  if (ctx) {
#if OPENSSL_VERSION_NUMBER >= 0x1010000fL
    HMAC_CTX_free(ctx);
#else
    HMAC_CTX_cleanup(ctx);
    free(ctx);
#endif
  }
  return resutl;
}

std::string SHA256Hash(const std::string& source) {
  std::string result;
  result.resize(32);
  SHA256((const uint8_t*)source.c_str(), source.length(), (uint8_t*)&result[0]);
  return result;
}

void HMAC_SHA(const unsigned char* text,
              int text_len,
              const unsigned char* key,
              int key_len,
              unsigned char* digest) {
#if OPENSSL_VERSION_NUMBER >= 0x1010000fL
  HMAC_CTX* ctx = HMAC_CTX_new();
  if (!ctx) {
    return SG_ERR_NOMEM;
  }
#else
  HMAC_CTX* ctx = (HMAC_CTX*)malloc(sizeof(HMAC_CTX));
  if (!ctx) {
    return;
  }
  HMAC_CTX_init(ctx);
#endif
  do {
    if (HMAC_Init_ex(ctx, key, key_len, EVP_sha1(), 0) != 1) {
      break;
    }
    HMAC_Update(ctx, text, text_len);

    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int len = 0;
    HMAC_Final(ctx, md, &len);
    BOOST_VERIFY(len == 20);
    memcpy(digest, md, len);
  } while (false);

  if (ctx) {
#if OPENSSL_VERSION_NUMBER >= 0x1010000fL
    HMAC_CTX_free(ctx);
#else
    HMAC_CTX_cleanup(ctx);
    free(ctx);
#endif
  }
}

std::string EncryptByAESGCM(const std::string &key, int64_t iv,
                     const std::string &source, const std::string &addata) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  std::string iv_str;
  iv_str.resize(12);
  iv_str[11] = (uint8_t)iv;
  iv_str[10] = (uint8_t)(iv >> 8);
  iv_str[9] = (uint8_t)(iv >> 16);
  iv_str[8] = (uint8_t)(iv >> 24);
  iv_str[7] = (uint8_t)(iv >> 32);
  iv_str[6] = (uint8_t)(iv >> 40);
  iv_str[5] = (uint8_t)(iv >> 48);
  iv_str[4] = (uint8_t)(iv >> 56);
  EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN , iv_str.length(),
                      nullptr);
  EVP_EncryptInit_ex(ctx, nullptr, nullptr, (const uint8_t *)key.c_str(),
                     (const uint8_t *)iv_str.c_str());
  //添加add ， 第二个参数为null
  int out_len = 0;
  EVP_EncryptUpdate(ctx, nullptr, &out_len, (const uint8_t*)addata.c_str(), addata.length());

  const uint8_t *base_source = (const uint8_t*)source.c_str();
  uint8_t result_buffer[1024];
  std::string result;
  //128 字节一轮
  for (int i = 0; i < source.length();) {
    out_len = 0;
    int input_size =
        (source.length() > (i + 128)) ? 128 : (source.length() - i);
    EVP_EncryptUpdate(ctx, result_buffer, &out_len, base_source + i,
                      input_size);
    result.append((const char*)result_buffer, out_len);
    i += 128;
  }
  EVP_EncryptFinal_ex(ctx, result_buffer, &out_len);
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, result_buffer);
  result.append((const char*)result_buffer, 16);
  EVP_CIPHER_CTX_free(ctx);
  return result;
}

std::string DecryptByAESGCM(const std::string& key,
                            int64_t iv,
                            const std::string& source,
                            const std::string& addata) {
  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  std::string iv_str;
  iv_str.resize(12);
  iv_str[11] = (uint8_t)iv;
  iv_str[10] = (uint8_t)(iv >> 8);
  iv_str[9] = (uint8_t)(iv >> 16);
  iv_str[8] = (uint8_t)(iv >> 24);
  iv_str[7] = (uint8_t)(iv >> 32);
  iv_str[6] = (uint8_t)(iv >> 40);
  iv_str[5] = (uint8_t)(iv >> 48);
  iv_str[4] = (uint8_t)(iv >> 56);
  EVP_DecryptInit(ctx, EVP_aes_256_gcm(), nullptr, nullptr);
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_str.length(), nullptr);
  EVP_DecryptInit(ctx, nullptr, (const uint8_t*)key.c_str(),
                     (const uint8_t*)iv_str.c_str());

  int out_len = 0;
  EVP_DecryptUpdate(ctx, nullptr, &out_len, (const uint8_t*)addata.c_str(),
                    addata.length());


  const uint8_t* base_source = (const uint8_t*)source.c_str();
  uint8_t result_buffer[1024];
  std::string result;
  int32_t data_len = source.length() - 16;
  // 128 字节一轮
  for (int i = 0; i < data_len;) {
    out_len = 0;
    int input_size =
      (data_len > (i + 128)) ? 128 : (data_len - i);
    EVP_DecryptUpdate(ctx, result_buffer, &out_len, base_source + i,
                      input_size);
    result.append((const char*)result_buffer, out_len);
    i += 128;
  }
  EVP_DecryptFinal_ex(ctx, result_buffer, &out_len);
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, result_buffer);
  EVP_CIPHER_CTX_free(ctx);
  return result;
}

#endif
END_NAMESPACE_CRYPTO


