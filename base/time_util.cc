#include "time_util.h"

BEGIN_NAMESPACE_TIME

std::chrono::steady_clock::time_point g_base_tick = std::chrono::steady_clock::now ();
uint64_t GetTimestamp() {
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}

uint64_t GetTimestampMsec() {
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

std::tm MakeTime(uint64_t seconds) {
  auto timestamp = std::chrono::system_clock::from_time_t(seconds);
  auto tmp = std::chrono::system_clock::to_time_t(timestamp);
  std::tm result;
#ifdef OS_WIN
  localtime_s(&result, &tmp);
#else
  localtime_r(&tmp, &result);
#endif
  return result;
}

uint64_t GetTickCount2() {
  auto now = std::chrono::steady_clock::now ();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now - g_base_tick).count ();
}


END_NAMESPACE_TIME

