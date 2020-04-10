#include "message_loop_impl.h"
#include <boost/assert.hpp>
BEGIN_NAMESPACE_LOOPER
std::shared_ptr<MessagePump> MessageLoopImpl::GetMessagePump() {
  throw std::logic_error("The method or operation is not implemented.");
}

END_NAMESPACE_LOOPER
