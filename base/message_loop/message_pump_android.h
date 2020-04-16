#ifndef _BASE_MESSAGE_PUMP_ANDROID_H_
#define _BASE_MESSAGE_PUMP_ANDROID_H_
#include "message_pump_impl.h"
#include <base_header.h>

BEGIN_NAMESPACE_LOOPER

class MessagePumpAndroid :public MessagePumpImpl {
public:
  MessagePumpAndroid();
  ~MessagePumpAndroid();

public:

protected:
  void Wakeup(uint64_t expired_time) override;

  void DoRun() override;


  void DoStop() override;

private:
};

END_NAMESPACE_LOOPER
#endif
