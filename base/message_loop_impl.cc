#include "message_loop_impl.h"
#include "message_pump_default.h"
#include <boost/assert.hpp>

BEGIN_NAMESPACE_LOOPER
#define WORK_THREAD_COUNT 4

MessageLoopImpl::MessageLoopImpl(const std::string& name, int32_t thread_count) {
  pump_ = std::make_shared<MessagePumpDefatlt>(name, thread_count);
}

MessageLoopImpl::~MessageLoopImpl() {
  if (pump_) {
    pump_->Stop();
  }
}

std::shared_ptr<MessagePump> MessageLoopImpl::GetMessagePump() {
  return pump_;
}

bool MessageLoopImpl::CallOnValidThread() {
  return pump_ == MessagePumpDefatlt::CurrentPump();
}


std::shared_ptr<MessagePump> MessageLoop::CurrentMessagePump() {
  return MessagePumpDefatlt::CurrentPump();
}


class MessageLooperManager {
public:
  MessageLooperManager() {
    loopers_[kMessagePumpTypeIO] = new MessageLoopImpl("IO", 1);
    loopers_[kMessagePumpTypeWork] = new MessageLoopImpl("Work", WORK_THREAD_COUNT);
    loopers_[kMessagePumpTypeFile] = new MessageLoopImpl("File", 1);
  }
  ~MessageLooperManager() {
    for (auto& item : loopers_) {
      item.second->GetMessagePump()->Stop();
      delete item.second;
    }
    loopers_.clear();
  }

  std::shared_ptr<MessagePump> GetMessagePumpByType(MessagePumpType type) {
    auto it = loopers_.find(type);
    if (it == loopers_.end()) {
      return nullptr;
    }
    return it->second->GetMessagePump();
  }

  //loopers_  操作不用加锁，统一由使用者加锁
  std::map<MessagePumpType, MessageLoopImpl*> loopers_;
};

class EmptyMessagePump : public MessagePump {

public:
  const char* name() override {
    return "";
  }


  void Run() override {
  }


  void Stop() override {
  }


  uint64_t PostRunable(Runnable runable) override {
    return -1;
  }


  uint64_t PostRunable(Runnable runable, uint64_t delay) override {
    return -1;
  }


  bool Cancel(uint64_t id) override {
    return false;
  }

};

std::mutex* g_loop_manager_mutex = new std::mutex;
MessageLooperManager* g_loop_manager = nullptr;
std::shared_ptr<MessagePump> s_empty_pump = std::make_shared<EmptyMessagePump>();


void MessageLoop::InitMessageLoop() {
  std::lock_guard<std::mutex> lock(*g_loop_manager_mutex);
  if (nullptr != g_loop_manager) {
    return;
  }
  g_loop_manager = new MessageLooperManager;
}

void MessageLoop::UnintMessageLoop() {
  MessageLooperManager* tmp = nullptr;
  {
    std::lock_guard<std::mutex> lock(*g_loop_manager_mutex);
    tmp = g_loop_manager;
    g_loop_manager = nullptr;
  }
  delete tmp;
}

std::shared_ptr<MessagePump> GetMessagePumpByType(MessagePumpType type) {
  std::lock_guard<std::mutex> lock(*g_loop_manager_mutex);
  if (nullptr == g_loop_manager) {
    return s_empty_pump;
  }
  return g_loop_manager->GetMessagePumpByType(type);
}


std::shared_ptr<MessagePump> MessageLoop::IOMessagePump() {
  
  return GetMessagePumpByType(kMessagePumpTypeIO);
}

std::shared_ptr<MessagePump> MessageLoop::WorkMessagePump() {
  return GetMessagePumpByType(kMessagePumpTypeWork);
}

std::shared_ptr<MessagePump> MessageLoop::FileMessagePump() {
  return GetMessagePumpByType(kMessagePumpTypeFile);
}

END_NAMESPACE_LOOPER
