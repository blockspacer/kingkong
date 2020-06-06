#include "event_bus.h"

BEGIN_NAMESPACE_EVENT_BUS
class EventBus {
  struct RunableInfo {
    Runnable runable;
    int id;
  };

 public:
  int32_t RegisterEventBus(int32_t event_id, Runnable runable, int32_t pro) {
    return 0;
  }

  void UnregisterEventBus(int32_t id) {}

  void FireEvent(int32_t event_id, boost::any value) {}

private:
  //优先级事件的map
  std::map <int32_t, std::map<int32_t, std::vector<RunableInfo>>> event_map_;
};

EventBus* g_event_bus = new EventBus;


int32_t RegisterEventBus(int32_t event_id, Runnable runable, int32_t pro) {
  return g_event_bus->RegisterEventBus(event_id, std::move(runable), pro);
}

void UnregisterEventBus(int32_t id) {
  return g_event_bus->UnregisterEventBus(id);
}

void FireEvent(int32_t event_id, boost::any value) {
  return g_event_bus->FireEvent(event_id, std::move(value));
}

END_NAMESPACE_EVENT_BUS


