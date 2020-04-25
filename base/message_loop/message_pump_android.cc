#include "message_pump_android.h"
#include "log.h"
#include "time_util.h"
#include "thread_util.h"
#include <boost/assert.hpp>
#include <boost/timer.hpp>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>

BEGIN_NAMESPACE_LOOPER

#ifndef TFD_TIMER_ABSTIME
#define TFD_TIMER_ABSTIME (1 << 0)
#endif

constexpr int64_t kNanosecondsPerSecond = 1000 * 1000 * 1000;

// See sys/timerfd.h
int timerfd_create(int clockid, int flags) {
  return syscall(__NR_timerfd_create, clockid, flags);
}

int timerfd_settime(int ufc,
                    int flags,
                    const struct itimerspec* utmr,
                    struct itimerspec* otmr) {
  return syscall(__NR_timerfd_settime, ufc, flags, utmr, otmr);
}

int DelayedLooperCallback(int fd, int events, void* data) {
  if (events & ALOOPER_EVENT_HANGUP)
    return 0;

  DCHECK(events & ALOOPER_EVENT_INPUT);
  MessagePumpAndroid* pump = reinterpret_cast<MessagePumpAndroid*>(data);
  pump->WorkWrapper();
  return 1;  // continue listening for events
}

MessagePumpAndroid::MessagePumpAndroid() : MessagePumpImpl("Main") {
  BASE_THREAD::SetThreadTls(this);
  BASE_THREAD::SetCurrentThreadName(name());

  delayed_fd_ = timerfd_create(CLOCK_MONOTONIC, O_NONBLOCK | O_CLOEXEC);
  looper_ = ALooper_forThread();
  ALooper_addFd(looper_,delayed_fd_,ALOOPER_POLL_CALLBACK,ALOOPER_EVENT_INPUT,&DelayedLooperCallback,this);
}

void MessagePumpAndroid::WorkWrapper() {
  uint64_t value;
  int ret = read(delayed_fd_, &value, sizeof(value));
  // TODO(mthiesse): Figure out how it's possible to hit EAGAIN here.
  // According to http://man7.org/linux/man-pages/man2/timerfd_create.2.html
  // EAGAIN only happens if no timer has expired. Also according to the man page
  // poll only returns readable when a timer has expired. So this function will
  // only be called when a timer has expired, but reading reveals no timer has
  // expired...
  // Quit() and ScheduleDelayedWork() are the only other functions that touch
  // the timerfd, and they both run on the same thread as this callback, so
  // there are no obvious timing or multi-threading related issues.
  DCHECK(ret >= 0 || errno == EAGAIN);
  DoWork();
}

MessagePumpAndroid::~MessagePumpAndroid() {
  ALooper_removeFd(looper_, delayed_fd_);
  ALooper_release(looper_);
  close(delayed_fd_);
}

void MessagePumpAndroid::Wakeup(uint64_t expired_time) {
  auto now = BASE_TIME::GetTickCount2();
  int64_t nanos = 0;
  if (expired_time > now) {
    nanos = (expired_time - now)*1000*1000;
  }
  struct itimerspec ts;
  ts.it_interval.tv_sec = 0;  // Don't repeat.
  ts.it_interval.tv_nsec = 0;
  ts.it_value.tv_sec = nanos / kNanosecondsPerSecond;
  ts.it_value.tv_nsec = nanos % kNanosecondsPerSecond;

  timerfd_settime(delayed_fd_, TFD_TIMER_ABSTIME, &ts, nullptr);
}

void MessagePumpAndroid::DoRun() {
}

void MessagePumpAndroid::DoStop() {
  
}


END_NAMESPACE_LOOPER
