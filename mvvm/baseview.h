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
  void BindVM(ViewModelType type);
  void UnBindVM();

public:
  //VM 已经创建，VM 有属性改变的时候会调用这个函数
  virtual void OnPropertyChanged(const std::string& property_name, boost::any& value) = 0;

protected:
  std::shared_ptr<ViewModel> vm_;

};

END_NAMESPACE_FRAME

#endif
