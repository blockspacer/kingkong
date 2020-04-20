#include "at_exit.h"
#include "log.h"

BEGIN_NAMESPACE_UTIL

static AtExitManager* g_top_manager = nullptr;

AtExitManager::AtExitManager() {
  if (nullptr != g_top_manager) {
    LogFatal << g_top_manager;
  }
  g_top_manager = this;
}

AtExitManager::~AtExitManager() {
  std::stack<AtExitCallback> tasks;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    tasks.swap(stack_);
  }

  // Relax the cross-thread access restriction to non-thread-safe RefCount.
  // It's safe since all other threads should be terminated at this point.

  while (!tasks.empty()) {
    auto& callback = tasks.top();
    callback();
    tasks.pop();
  }
  g_top_manager = nullptr;
}

void AtExitManager::RegisterCallback(AtExitCallback callback) {
  if (nullptr == g_top_manager) {
    LogFatal << "g_top_manager: null， must use temp object";
    return;
  }
  std::lock_guard<std::mutex> lock(g_top_manager->mutex_);
  g_top_manager->stack_.push(std::move(callback));
}



END_NAMESPACE_UTIL

