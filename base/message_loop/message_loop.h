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

  //不能执行耗时任务，否则会卡主UI
  static std::shared_ptr<MessagePump> UIMessagePump();
  //可以保证任务的执行顺序
  static std::shared_ptr<MessagePump> IOMessagePump();
  //可以保证任务的执行顺序
  static std::shared_ptr<MessagePump> FileMessagePump();
  //Work 内部是一个线程池，所以只能用来执行一个完整的任务，比如计算文件MD5，循环读文件等。如果任务有先后顺序的话，不要放到work线程，
  //work 线程不保证任务的执行顺序
  static std::shared_ptr<MessagePump> WorkMessagePump();
};


END_NAMESPACE_LOOPER


#endif
