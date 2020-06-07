#include "string_util.h"
#include <boost/locale/encoding.hpp>
#include <boost/tokenizer.hpp>   
#include <boost/algorithm/string.hpp> 

BEGIN_NAMESPACE_STRING

std::string EMPTY_STRING;

char s_hex_data_map[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                          '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static bool byte_to_hex(unsigned char data, char *buf, int len = 2) {
  if (NULL == buf || len < 2) {
    return false;
  }

  unsigned char h1 = (data & 0xF0) >> 4;
  unsigned char h2 = data & 0xF;

  buf[0] = s_hex_data_map[h1];
  buf[1] = s_hex_data_map[h2];

  return true;
}

std::string UnicodeToUtf8(const std::wstring& value) {
  return boost::locale::conv::from_utf(value, "UTF-8");
}

std::wstring Utf8ToUnicode(const std::string& value) {
  return boost::locale::conv::utf_to_utf<wchar_t>(value);
}

std::string UnicodeToGBK(const std::wstring& value) {
  return boost::locale::conv::from_utf(value, "GBK");
}

std::wstring GBKToUnicode(const std::string& value) {
  return boost::locale::conv::to_utf<wchar_t>(value, "GBK");
}

std::string Utf8ToGBK(const std::string& value) {
  return boost::locale::conv::between(value, "GBK", "UTF-8");
}

std::string GBKToUTF8e(const std::string& value) {
  return boost::locale::conv::between(value, "UTF-8", "GBK");
}

std::vector<std::string> Split(const std::string& src,
                               const std::string& token,
                               bool allow_empty /*= false*/) {
  std::vector<std::string> split_strings;
  boost::split(split_strings, src, boost::is_any_of(token));
  if (!allow_empty) {
    //去掉empty
    for (auto iter = split_strings.begin(); iter != split_strings.end();) {
      if (iter->empty()) {
        iter = split_strings.erase(iter);
      } else {
        iter++;
      }
    }
  }
  return split_strings;
}

std::vector<std::string> Split(const std::string &input, int first_length,
                               int second_length, int third_length /*= -1*/) {
  std::vector<std::string> result;
  result.push_back(input.substr(0, first_length));
  result.push_back(input.substr(first_length, second_length));
  if (third_length > -1) {
    result.push_back(input.substr(first_length + second_length, third_length));
  }
  return result;
}

std::string BinToHex(const std::string &bin) {
  std::string result(bin.length() * 2, '*');
  char *buff = (char *)result.c_str();
  for (size_t i = 0; i < bin.length(); i++) {
    byte_to_hex(*(bin.c_str() + i), (buff + i * 2));
  }
  return result;
}

END_NAMESPACE_STRING


