#ifndef _BASE_VIEW_H_
#define _BASE_VIEW_H_
#include <memory>
#include <mvvm/mvvm_base.h>
#include <mvvm/viewmodel.h>
#include <google/protobuf/message.h>

BEGIN_NAMESPACE_FRAME

class BaseView {
public:
  virtual ~BaseView();

public:
  void  CreateVM(int32_t type);
  void BindVM();
  void UnBindVM();

public:
  //VM 已经创建，VM 有属性改变的时候会调用这个函数
  virtual void OnPropertyChanged(int32_t property_id, const ::google::protobuf::Message* value) = 0;
  virtual void OnEventFired(int32_t event_id, const ::google::protobuf::Message* value) = 0;

protected:
  std::shared_ptr<ViewModel> vm_;

};

END_NAMESPACE_FRAME

#endif
