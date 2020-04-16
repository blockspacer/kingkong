#ifndef _BASE_MESSAGE_LOOPER_H_
#define _BASE_MESSAGE_LOOPER_H_
#include <base/message_loop/message_pump.h>

BEGIN_NAMESPACE_LOOPER

enum MessagePumpType {
  kMessagePumpTypeBegin,
  kMessagePumpTypeUI = kMessagePumpTypeBegin,
  kMessagePumpTypeIO,
  //work pump 背后是一个线程池，可以同时处理多个耗时任务
  kMessagePumpTypeWork,
  kMessagePumpTypeFile,
  kMessagePumpTypeEnd
};


class MessageLoop {
public:
  MessageLoop() = default;
  virtual ~MessageLoop() = default;

  virtual std::shared_ptr<MessagePump> GetMessagePump() = 0;
  virtual bool CallOnValidThread() = 0;

  static std::shared_ptr<MessagePump> CurrentMessagePump();

  //初始化和返初始化都需要在主线程调用
  static void InitMessageLoop();
  static void UnintMessageLoop();

  static std::shared_ptr<MessagePump> UIMessagePump();
  static std::shared_ptr<MessagePump> IOMessagePump();
  static std::shared_ptr<MessagePump> WorkMessagePump();
  static std::shared_ptr<MessagePump> FileMessagePump();
};


END_NAMESPACE_LOOPER


#endif
