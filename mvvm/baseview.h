#ifndef _BASE_VIEW_H_
#define _BASE_VIEW_H_
#include <memory>
#include <mvvm_base.h>
#include <viewmodel.h>

BEGIN_NAMESPACE_FRAME

class BaseView {
public:
  virtual ~BaseView();

public:
  void BindVM(int32_t type);
  void UnBindVM();

public:
  //VM 已经创建，VM 有属性改变的时候会调用这个函数
  virtual void OnPropertyChanged(int32_t property_id, const boost::any& before_value, const boost::any& after_value) = 0;

protected:
  std::shared_ptr<ViewModel> vm_;

};

END_NAMESPACE_FRAME

#endif
