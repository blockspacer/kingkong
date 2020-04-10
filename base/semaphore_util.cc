#include "semaphore_util.h"

BEGIN_NAMESPACE_THREAD
class SemaphoreImpl :public Semaphore {
public:
  void Signal() override {
   std::unique_lock<std::mutex> lock(mutex_);
   ++count_;
   cv_.notify_one();
  }

  void Wait() override {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [=] { return count_ > 0; });
    --count_;
  }

  void Wait(uint64_t timeout) override {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait_for(lock, std::chrono::milliseconds(timeout), [=] {
      if (count_ > 0) {
        count_--;
        return true;
      }
      return false;
    });
  }

private:
  std::mutex mutex_;
  std::condition_variable cv_;
  int count_ = 0;
};

std::unique_ptr<Semaphore> CreateSigalSemaphore() {
  return std::make_unique<SemaphoreImpl>();
}

END_NAMESPACE_THREAD


