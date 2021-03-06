﻿#include "baseview.h"
#include "viewmodel.h"

BEGIN_NAMESPACE_FRAME
BaseView::~BaseView() {

}

void BaseView::CreateVM(int32_t type) {
  vm_ = ViewModel::Create(type);
  //给UI 一个机会绑定事件和属性
  vm_->BindPropertyChanged([this](int32_t property_id, const ::google::protobuf::Message* after_value) {
    OnPropertyChanged(property_id, after_value);
  });

  vm_->BindEvent([this](int32_t event_id, const ::google::protobuf::Message* value) {
    OnEventFired(event_id, value);
  });
}

void BaseView::BindVM() {
  //通知VM，上层已经绑定完成了，可以开始抛属性了
  vm_->NotifyBind(this);
}

void BaseView::UnBindVM() {
  vm_->NotifyUnBind();
}


END_NAMESPACE_FRAME

