#include "baseview.h"
#include "viewmodel.h"

BEGIN_NAMESPACE_FRAME
BaseView::~BaseView() {

}

void BaseView::BindVM(ViewModelType type) {
  vm_ = ViewModel::Create(type);
  //给UI 一个机会绑定事件和属性
  vm_->BindPropertyChanged([this](const std::string& property_name, boost::any& value) {
    OnPropertyChanged(property_name, value);
  });
  //通知VM，上层已经绑定完成了，可以开始抛属性了
  vm_->NotifyBind();
}


void BaseView::UnBindVM() {
  vm_->NotifyUnBind();
}


END_NAMESPACE_FRAME

