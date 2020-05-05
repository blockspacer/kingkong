#include "string_util.h"
#include <boost/locale/encoding.hpp>
#include <boost/tokenizer.hpp>   
#include <boost/algorithm/string.hpp> 

BEGIN_NAMESPACE_STRING

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

END_NAMESPACE_STRING

