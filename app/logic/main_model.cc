#include "main_model.h"
#include "logic_define.h"

 MainModel::MainModel() {}

 void MainModel::HandleEvent(int32_t event, const ::google::protobuf::Message* value)  {
   BEGIN_HANDLE_EVENT(event)
    HANDLE_EVENT(Main::Event::kEventLogin, HandleLogin)
    HANDLE_EVENT(Main::Event::kEventLogout, HandleLogout)
   END_HANDLE_EVENT()
 }

 void MainModel::HandleLogin(const ::google::protobuf::Message* value) {
    mvvm::mvvm_ParamString param;
   param.set_value("login sucess");
   FireActionResult(Main::Event::kEventLogin, &param);
 }

 void MainModel::HandleLogout(const ::google::protobuf::Message* value) {
    mvvm::mvvm_ParamInt32 param;
   param.set_value(1);
   FireActionResult(Main::Event::kEventLogout, &param);
 }
