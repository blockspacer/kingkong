#ifndef _BASE_MESSAGE_LOOP_IMPL_H_
#define _BASE_MESSAGE_LOOP_IMPL_H_
#include "message_loop.h"
#include <base_header.h>

BEGIN_NAMESPACE_LOOPER

class MessageLoopImpl :public MessageLoop {
public:
  MessageLoopImpl(const std::string& name, int32_t thread_count);
  ~MessageLoopImpl();

  std::shared_ptr<MessagePump> GetMessagePump() override;
  bool CallOnValidThread() override;

private:
  std::shared_ptr<MessagePump> pump_;
};

END_NAMESPACE_LOOPER
#endif
