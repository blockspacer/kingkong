#include "main_viewmodel.h"

MainViewModel::MainViewModel(): MVVM_FRAME::ViewModel(kViewModelMain) {
}

void MainViewModel::OnAttach() {
  BEGIN_SUBSCRIE_ACTION(kModelMain)
    SUBSCRIE_ACTION(Main::kActionLogin, &MainViewModel::OnLoginComplete);
    SUBSCRIE_ACTION(Main::kActionLogout, &MainViewModel::OnLogoutComplete);
  END_SUBSCRIE_ACTION()
      //初始化UI 数据
    {
      XMVVM::XMVVM_ParamString param;
      param.set_value("init username");
      FireProperty(Main::Properties::kPropertyUserName, &param);
    }
    
    {
      XMVVM::XMVVM_ParamString param;
      param.set_value("init passwd");
      FireProperty(Main::Properties::kPropertyPasswd, &param);
    }
}

void MainViewModel::OnDetach() {

}

void MainViewModel::OnEventFired(int event, const ::google::protobuf::Message* value) {
  BEGIN_HANDLE_EVENT(event)
    HANDLE_EVENT(Main::Event::kEventLogin, HandleLogin);
    HANDLE_EVENT(Main::Event::kEventLogout, HandleLogout);
  END_HANDLE_EVENT()
}

void MainViewModel::HandleLogin(const ::google::protobuf::Message* value) {
  if (auto model = MVVM_FRAME::Model::ModelOf(kModelMain)) {
    model->Call(Main::Action::kActionLogin, value);
  }
}

void MainViewModel::HandleLogout(const ::google::protobuf::Message* value) {
  if (auto model = MVVM_FRAME::Model::ModelOf(kModelMain)) {
    model->Call(Main::Action::kActionLogout, value);
  }
}

void MainViewModel::OnLoginComplete(const ::google::protobuf::Message* value) {
  FireProperty(Main::Properties::kPropertyUserName, value);
}

void MainViewModel::OnLogoutComplete(const ::google::protobuf::Message* value) {
  FireProperty(Main::Properties::kPropertyPasswd, value);
}
