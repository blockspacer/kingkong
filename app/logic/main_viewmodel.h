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
  void OnEventFired(int event, const ::google::protobuf::Message* value) override;

private:
  void HandleLogin(const ::google::protobuf::Message* value);
  void HandleLogout(const ::google::protobuf::Message* value);

  //处理model 的动作回调
  void OnLoginComplete(int32_t event_id, const ::google::protobuf::Message* value);

};



#endif
