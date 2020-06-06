#ifndef _EVENT_BUS_H_
#define _EVENT_BUS_H_
#include <base/base_header.h>
#include <boost/any.hpp>
BEGIN_NAMESPACE_EVENT_BUS

using Runnable = std::function<bool(boost::any value)>;

//指定 一个优先级订阅一个事件
int32_t RegisterEventBus(int32_t event_id, Runnable runable, int32_t pro);
void UnregisterEventBus(int32_t id);

void FireEvent(int32_t event_id, boost::any value);

END_NAMESPACE_EVENT_BUS
#endif
