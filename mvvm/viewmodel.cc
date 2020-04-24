#include "viewmodel.h"
#include <boost/assert.hpp>
BEGIN_NAMESPACE_FRAME

std::map< int32_t, ViewModel::ViewModelBuilder> ViewModel::vm_builder_;

ViewModel::ViewModel(int32_t viewmode_type): viewmode_type_(viewmode_type) {}

ViewModel::~ViewModel() {
  for (auto& item : subscribe_ids_) {
    auto model = Model::ModelOf(item.second);
    if (nullptr == model) {
      continue;;
    }
    model->UnSubscribeActionResult(item.first);
  }
  subscribe_ids_.clear();
}

void ViewModel::BindPropertyChanged(PropertyChangedDelegate delegate) {
  property_changed_delegate_ = std::move(delegate);
}

void ViewModel::NotifyPropertyChanged(int32_t property_id, boost::any& value) {
  properties_[property_id] = value;
}

void ViewModel::NotifyPropertyChanged(int32_t property_id, boost::any&& value) {
  properties_[property_id] = std::move(value);
}

void ViewModel::HandleEvent(int32_t event, const boost::any& value) {
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
  OnDetach();
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

void ViewModel::FireProperty(int32_t property_id, const boost::any& value) {
  if (property_changed_delegate_) {
    auto iter = properties_.find(property_id);
    if (iter != properties_.end()) {
      property_changed_delegate_(property_id, iter->second, value);
    }
    else {
      property_changed_delegate_(property_id, boost::any(), value);
    }
    properties_[property_id] = value;
  }
}
END_NAMESPACE_FRAME


