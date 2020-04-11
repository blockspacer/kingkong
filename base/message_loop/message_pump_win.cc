#include "message_pump_win.h"
#include "log.h"
#include "time_util.h"
#include "thread_util.h"
#include <boost/assert.hpp>
#include <boost/timer.hpp>

BEGIN_NAMESPACE_LOOPER

MessagePumpWin::MessagePumpWin() : MessagePumpImpl("Main") {
  BASE_THREAD::SetThreadTls(this);
}

MessagePumpWin::~MessagePumpWin() {
  if (!stoped()) {
   
  }
}

void MessagePumpWin::Wakeup(uint64_t expired_time) {
  auto now = BASE_TIME::GetTickCount2();
  if (expired_time > now) {
   
  } else {
   
  }
}

void MessagePumpWin::DoRun() {
}

void MessagePumpWin::DoStop() {
}


END_NAMESPACE_LOOPER
