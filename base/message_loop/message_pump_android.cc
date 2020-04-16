#include "message_pump_android.h"
#include "log.h"
#include "time_util.h"
#include "thread_util.h"
#include <boost/assert.hpp>
#include <boost/timer.hpp>
BEGIN_NAMESPACE_LOOPER

MessagePumpAndroid::MessagePumpAndroid() : MessagePumpImpl("Main") {
  BASE_THREAD::SetThreadTls(this);
  BASE_THREAD::SetCurrentThreadName(name());
  


}

MessagePumpAndroid::~MessagePumpAndroid() {
}

void MessagePumpAndroid::Wakeup(uint64_t expired_time) {
  auto now = BASE_TIME::GetTickCount2();
 
}

void MessagePumpAndroid::DoRun() {
}

void MessagePumpAndroid::DoStop() {
  
}


END_NAMESPACE_LOOPER
