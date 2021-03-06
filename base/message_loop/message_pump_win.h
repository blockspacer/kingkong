﻿#ifndef _BASE_MESSAGE_PUMP_WIN_H_
#define _BASE_MESSAGE_PUMP_WIN_H_
#include <base/message_loop/message_pump_impl.h>
#include <base/base_header.h>

BEGIN_NAMESPACE_LOOPER

class MessagePumpWin :public MessagePumpImpl {
public:
  MessagePumpWin();
  ~MessagePumpWin();

public:
  void Work();

protected:
  void Wakeup(uint64_t expired_time) override;

  void DoRun() override;

  void Stop() override;

  void DoStop() override;

private:
};

END_NAMESPACE_LOOPER
#endif
