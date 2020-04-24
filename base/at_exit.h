#ifndef _AT_EXIT_UTIL_H_
#define _AT_EXIT_UTIL_H_
#include <base/base_header.h>
#include <stack>

BEGIN_NAMESPACE_UTIL
class AtExitManager {
public:
  using AtExitCallback = std::function<void()>;

  AtExitManager();
  ~AtExitManager();

  static void RegisterCallback(AtExitCallback callback);

private:
  std::mutex mutex_;
  std::stack< AtExitCallback> stack_;
};

END_NAMESPACE_UTIL
#endif
