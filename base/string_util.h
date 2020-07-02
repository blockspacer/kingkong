#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_
#include <base/base_header.h>
#include <chrono>
#include <ctime>

BEGIN_NAMESPACE_STRING


extern std::string EMPTY_STRING;
#ifdef WIN32
//unicode utf8 互转
std::string UnicodeToUtf8(const std::wstring& value);
std::wstring Utf8ToUnicode(const std::string& value);

//gbk unicode 互转
std::string UnicodeToGBK(const std::wstring& value);
std::wstring GBKToUnicode(const std::string& value);

//utf8 gbk 互转
std::string Utf8ToGBK(const std::string& value);
std::string GBKToUTF8e(const std::string& value);
#endif

//字符串分割
std::vector<std::string> Split(const std::string& src, const std::string& token, bool allow_empty = false);

//二进制转16 进制
std::string BinToHex(const char *bin, int32_t len);
std::string BinToHex(const std::string &bin);
std::string HexToBin(const std::string& hex);


//字符串分割
std::vector<std::string> Split(const std::string &input, int first_length,
                                    int second_length, int third_length = -1);



std::string URLEncode(const std::string &str);
END_NAMESPACE_STRING
#endif
