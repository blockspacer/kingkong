#include "main_viewmodel.h"

MainViewModel::MainViewModel(): MVVM_FRAME::ViewModel(VMDefine::kViewModelMain) {
}

void MainViewModel::OnAttach() {
  BEGIN_SUBSCRIE_EVENT(VMDefine::kModelMain)
    SUBSCRIE_EVENT(Main::Event::kEventLogin, &MainViewModel::OnLoginComplete);
  END_SUBSCRIE_EVENT()
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
  if (auto model = MVVM_FRAME::Model::ModelOf(VMDefine::kModelMain)) {
    model->Call(Main::Event::kEventLogin, value);
  }
}

void MainViewModel::HandleLogout(const ::google::protobuf::Message* value) {
  if (auto model = MVVM_FRAME::Model::ModelOf(VMDefine::kModelMain)) {
    model->Call(Main::Event::kEventLogout, value);
  }
}

void MainViewModel::OnLoginComplete(int32_t event_id, const ::google::protobuf::Message* value) {

}
