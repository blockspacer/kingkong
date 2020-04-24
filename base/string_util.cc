#include "string_util.h"
#include <boost/locale/encoding.hpp>

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


END_NAMESPACE_STRING
