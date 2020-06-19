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
  return BinToHex(bin.c_str(), bin.length());
}

std::string BinToHex(const char *bin, int32_t len) {
  std::string result(len * 2, '*');
  char *buff = (char *)result.c_str();
  for (size_t i = 0; i < len; i++) {
    byte_to_hex(*(bin + i), (buff + i * 2));
  }
  return result;
}


static int hex_to_int(uint8_t ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  } else if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  } else if (ch >= 'a' && ch <= 'f') {
    return ch - 'a' + 10;
  } else {
    return -1;
  }
}

static int hex_string_to_bytes(const char *hex_buffer, unsigned int hex_len,
                               unsigned char *buffer, unsigned int buffer_len) {
  if (NULL == hex_buffer || NULL == buffer) {
    return -1;
  }
  if (hex_len % 2 != 0) {
    return -1;
  }

  unsigned int nbytes = hex_len / 2;
  if (buffer_len < nbytes) {
    return -1;
  }

  for (unsigned int i = 0; i < nbytes; i++) {
    uint8_t c1 = hex_buffer[i * 2];
    int h1 = hex_to_int(c1);
    if (h1 < 0) {
      return -1;
    }

    uint8_t c2 = hex_buffer[i * 2 + 1];
    int h2 = hex_to_int(c2);
    if (h2 < 0) {
      return -1;
    }

    uint8_t b = (uint8_t)((h1 << 4) | h2);
    buffer[i] = b;
  }

  return nbytes;
}

std::string HexToBin(const std::string& hex) {
  std::string ret;
  ret.resize(hex.size() / 2);
  int len = hex_string_to_bytes(hex.c_str(), (unsigned int)hex.size(),
                                (uint8_t *)ret.c_str(), (unsigned int)ret.size());
  if (len == -1) {
    ret.erase();
  } else {
    ret.resize(len);
  }
  return ret;
}

char s_hex_data_map3[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                          '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
std::string URLEncode(const std::string &str) {
  std::string result = "";
  size_t length = str.length();
  for (size_t i = 0; i < length; i++) {
    if (isalnum((unsigned char)str[i]) || (str[i] == '.') || (str[i] == '~'))
      result += str[i];
    else if (str[i] == ' ')
      result += "+";
    else {
      result += '%';
      result += s_hex_data_map3[((unsigned char)str[i] >> 4) & 15];
      result += s_hex_data_map3[((unsigned char)str[i] & 15)];
    }
  }
  return result;
}

END_NAMESPACE_STRING


