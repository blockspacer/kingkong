#include "main_viewmodel.h"

MainViewModel::MainViewModel(): MVVM_FRAME::ViewModel(kViewModelMain) {
}

void MainViewModel::OnAttach() {
  BEGIN_SUBSCRIE_ACTION(kModelMain)
    SUBSCRIE_ACTION(Main::kActionLogin, &MainViewModel::OnLoginComplete);
    SUBSCRIE_ACTION(Main::kActionLogout, &MainViewModel::OnLogoutComplete);
  END_SUBSCRIE_ACTION()
  //初始化UI 数据
  FireProperty(Main::Properties::kPropertyUserName, "init username");
 
  Main::UserNameParam param;
  param.school = "init passwd";
  FireProperty(Main::Properties::kPropertyPasswd, param);
}

void MainViewModel::OnDetach() {

}

void MainViewModel::OnEventFired(int event, const boost::any& value) {
  BEGIN_HANDLE_EVENT(event)
    HANDLE_EVENT(Main::Event::kEventLogin, HandleLogin);
    HANDLE_EVENT(Main::Event::kEventLogout, HandleLogout);
  END_HANDLE_EVENT()
}

void MainViewModel::HandleLogin(const boost::any& value) {
  if (auto model = MVVM_FRAME::Model::ModelOf(kModelMain)) {
    model->Call(Main::Action::kActionLogin, value);
  }
}

void MainViewModel::HandleLogout(const boost::any& value) {
  if (auto model = MVVM_FRAME::Model::ModelOf(kModelMain)) {
    model->Call(Main::Action::kActionLogout, value);
  }
}

void MainViewModel::OnLoginComplete(const boost::any& value) {
  FireProperty(Main::Properties::kPropertyUserName, value);
}

void MainViewModel::OnLogoutComplete(const boost::any& value) {
  FireProperty(Main::Properties::kPropertyPasswd, value);
}
