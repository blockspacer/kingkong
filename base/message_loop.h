#ifndef _BASE_MESSAGE_LOOPER_H_
#define _BASE_MESSAGE_LOOPER_H_
#include "message_pump.h"
#include <base_header.h>

BEGIN_NAMESPACE_LOOPER

class MessageLoop {
public:
  explicit MessageLoop(const std::string& name);
  virtual ~MessageLoop() = default;

  virtual std::shared_ptr<MessagePump> GetMessagePump() = 0;
};

END_NAMESPACE_LOOPER
#endif
