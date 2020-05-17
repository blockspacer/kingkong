#include "viewmodel.h"
#include <boost/assert.hpp>
#include <pb/MvvmBasicParam.pb.h>

BEGIN_NAMESPACE_FRAME
std::map< int32_t, ViewModel::ViewModelBuilder> ViewModel::vm_builder_;

ViewModel::ViewModel(int32_t viewmode_type): viewmode_type_(viewmode_type) {}

ViewModel::~ViewModel() {
  UnSubscrieAllEvent();
}

void ViewModel::UnSubscrieAllEvent() {
  for (auto& item : subscribe_ids_) {
    auto model = Model::ModelOf(item.second);
    if (nullptr == model) {
      continue;
      ;
    }
    model->UnSubscribeEvent(item.first);
  }
  subscribe_ids_.clear();
}

void ViewModel::BindPropertyChanged(PropertyChangedDelegate delegate) {
  property_changed_delegate_ = std::move(delegate);
}

void ViewModel::BindEvent(EventFiredDelegate event_fired_delegate) {
  event_fired_delegate_ = std::move(event_fired_delegate);
}

void ViewModel::HandleEvent(int32_t event, const ::google::protobuf::Message* value) {
  OnEventFired(event, value);
}

int32_t ViewModel::viewmode_type() {
  return viewmode_type_;
}


void ViewModel::NotifyBind(void* context) {
  context_ = context;
  OnAttach();
}

void ViewModel::NotifyUnBind() {
  property_changed_delegate_ = nullptr;
  event_fired_delegate_ = nullptr;
  OnDetach();
  UnSubscrieAllEvent();
}

void ViewModel::RegisterViewModel(int32_t type, ViewModelBuilder builder) {
  vm_builder_[type] = std::move(builder);
}

std::shared_ptr<ViewModel> ViewModel::Create(int32_t type) {
  auto iter = vm_builder_.find(type);
  if (iter == vm_builder_.end()) {
    return nullptr;
  }
  return iter->second();
}

void ViewModel::SetProperty(int32_t property_id, int32_t value) {
  if (property_changed_delegate_) {
    mvvm::mvvm_ParamInt32 pb_value;
    pb_value.set_value(value);
    property_changed_delegate_(property_id, &pb_value);
  }
}

void ViewModel::SetProperty(int32_t property_id, int64_t value) {
  if (property_changed_delegate_) {
    mvvm::mvvm_ParamInt64 pb_value;
    pb_value.set_value(value);
    property_changed_delegate_(property_id, &pb_value);
  }
}

void ViewModel::SetProperty(int32_t property_id, bool value) {
  if (property_changed_delegate_) {
    mvvm::mvvm_ParamBool pb_value;
    pb_value.set_value(value);
    property_changed_delegate_(property_id, &pb_value);
  }
}

void ViewModel::SetProperty(int32_t property_id, const std::string& value) {
  if (property_changed_delegate_) {
    mvvm::mvvm_ParamString pb_value;
    pb_value.set_value(value);
    property_changed_delegate_(property_id, &pb_value);
  }
}

void ViewModel::SetProperty(int32_t property_id, const ::google::protobuf::Message* value) {
  if (property_changed_delegate_) {
      property_changed_delegate_(property_id, value);
  }
}


void ViewModel::FireEvent(int32_t event_id, int32_t value) {
  if (event_fired_delegate_) {
    mvvm::mvvm_ParamInt32 pb_value;
    pb_value.set_value(value);
    event_fired_delegate_(event_id, &pb_value);
  }
}

void ViewModel::FireEvent(int32_t event_id, int64_t value) {
  if (event_fired_delegate_) {
    mvvm::mvvm_ParamInt64 pb_value;
    pb_value.set_value(value);
    event_fired_delegate_(event_id, &pb_value);
  }
}

void ViewModel::FireEvent(int32_t event_id, bool value) {
  if (event_fired_delegate_) {
    mvvm::mvvm_ParamBool pb_value;
    pb_value.set_value(value);
    event_fired_delegate_(event_id, &pb_value);
  }
}

void ViewModel::FireEvent(int32_t event_id, const std::string& value) {
  if (event_fired_delegate_) {
    mvvm::mvvm_ParamString pb_value;
    pb_value.set_value(value);
    event_fired_delegate_(event_id, &pb_value);
  }
}

void ViewModel::FireEvent(int32_t event_id, const ::google::protobuf::Message* value) {
  if (event_fired_delegate_) {
    event_fired_delegate_(event_id, value);
  }
}

void ViewModel::RouteEventToUI(int32_t event_id, const ::google::protobuf::Message* value) {
  FireEvent(event_id, value);
}
END_NAMESPACE_FRAME


