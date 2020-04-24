#ifndef _MAIN_VIEWMODEL_H_
#define _MAIN_VIEWMODEL_H_
#include <logic_define.h>

class MainViewModel :public MVVM_FRAME::ViewModel {
public:
  MainViewModel();

  void OnAttach() override;
  void OnDetach() override;

  static std::shared_ptr<MVVM_FRAME::ViewModel> CreateViewModel() {
    return std::make_shared<MainViewModel>();
  }
  //处理 UI 的事件
  void OnEventFired(int event, const boost::any& value) override;

private:
  void HandleLogin(const boost::any& value);
  void HandleLogout(const boost::any& value);

  //处理model 的动作回调
  void OnLoginComplete(const boost::any& value);
  void OnLogoutComplete(const boost::any& value);

};



#endif
