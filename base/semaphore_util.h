#ifndef _SEMAPHORE_UTIL_H_
#define _SEMAPHORE_UTIL_H_
#include <base/base_header.h>
#include <condition_variable>
#include <chrono>
#include <ctime>

BEGIN_NAMESPACE_THREAD
class Semaphore {
public:
  virtual ~Semaphore() = default;

public:
  virtual void Signal() = 0;
  virtual void Wait() = 0;
  virtual void Wait(uint64_t timeout) = 0;
};

std::unique_ptr<Semaphore> CreateSigalSemaphore();

END_NAMESPACE_THREAD
#endif
