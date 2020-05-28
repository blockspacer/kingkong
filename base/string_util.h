#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_
#include <base/base_header.h>
#include <chrono>
#include <ctime>

BEGIN_NAMESPACE_STRING
//unicode utf8 互转
std::string UnicodeToUtf8(const std::wstring& value);
std::wstring Utf8ToUnicode(const std::string& value);

//gbk unicode 互转
std::string UnicodeToGBK(const std::wstring& value);
std::wstring GBKToUnicode(const std::string& value);

//utf8 gbk 互转
std::string Utf8ToGBK(const std::string& value);
std::string GBKToUTF8e(const std::string& value);

//字符串分割
std::vector<std::string> Split(const std::string& src, const std::string& token, bool allow_empty = false);

//二进制转16 禁止
std::string BinToHex(const std::string &bin);

//字符串分割
std::vector<std::string> Split(const std::string &input, int first_length,
                                    int second_length, int third_length = -1);

END_NAMESPACE_STRING
#endif
