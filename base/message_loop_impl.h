#ifndef _BASE_MESSAGE_LOOP_IMPL_H_
#define _BASE_MESSAGE_LOOP_IMPL_H_
#include "message_loop.h"
#include <base_header.h>

BEGIN_NAMESPACE_LOOPER

class MessageLoopImpl :public MessageLoop {
public:
  explicit MessageLoopImpl(const std::string& name);

  std::shared_ptr<MessagePump> GetMessagePump() override;

private:
};

END_NAMESPACE_LOOPER
#endif
