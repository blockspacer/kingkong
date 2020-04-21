#include "viewmodel.h"
#include <boost/assert.hpp>
BEGIN_NAMESPACE_FRAME

ViewModel::ViewModel(ViewModelType viewmode_type): viewmode_type_(viewmode_type) {}

ViewModel::~ViewModel() {
  for (auto& item : subscribe_ids_) {
    auto model = Model::ModelOf(item.second);
    if (nullptr == model) {
      continue;;
    }
    model->UnSubscribeEvent(item.first);
  }
  subscribe_ids_.clear();
}

void ViewModel::BindPropertyChanged(PropertyChangedDelegate delegate) {
  property_changed_delegate_ = std::move(delegate);
}

void ViewModel::NotifyPropertyChanged(const std::string& property_name, boost::any& value) {
  auto value_pointer =  boost::any_cast<VMParamBase>(&value);
  BOOST_VERIFY(value_pointer != nullptr);

  properties_[property_name] = value;
}

void ViewModel::NotifyPropertyChanged(const std::string& property_name, boost::any&& value) {
  auto value_pointer = boost::any_cast<VMParamBase>(&value);
  BOOST_VERIFY(value_pointer != nullptr);
  properties_[property_name] = std::move(value);
}

void ViewModel::HandleEvent(int event, boost::any& value) {
  OnEventFired(event, value);
}

ViewModelType ViewModel::viewmode_type() {
  return viewmode_type_;
}


void ViewModel::NotifyBind() {
  OnAttach();
}

void ViewModel::NotifyUnBind() {
  OnDetach();
}

void ViewModel::RegisterViewModel(ViewModelType type, ViewModelBuilder builder) {
  vm_builder_[type] = std::move(builder);
}

std::shared_ptr<ViewModel> ViewModel::Create(ViewModelType type) {
  auto iter = vm_builder_.find(type);
  if (iter == vm_builder_.end()) {
    return nullptr;
  }
  return iter->second();
}

void ViewModel::SubscribEvent(ModelType model_type,
                              int32_t event,
                              Model::SuscribeEventDelegate delegate) {
  auto model = Model::ModelOf(model_type);
  if (nullptr == model) {
    return;
  }
  int64_t id = model->SubscribeEvent(event, std::move(delegate));
  subscribe_ids_[id] = model_type;
}
END_NAMESPACE_FRAME


