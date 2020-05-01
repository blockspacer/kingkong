#include "main_model.h"
#include "logic_define.h"

 MainModel::MainModel() {}

 void MainModel::HandleAction(int32_t action, const ::google::protobuf::Message* value)  {
    BEGIN_HANDLE_ACTION(action)
      HANDLE_ACTION(Main::Action::kActionLogin, HandleLogin)
      HANDLE_ACTION(Main::Action::kActionLogout, HandleLogout)
    END_HANDLE_ACTION()
 }

 void MainModel::HandleLogin(const ::google::protobuf::Message* value) {
   XMVVM::XMVVM_ParamString param;
   param.set_value("login sucess");
   FireActionResult(Main::Action::kActionLogin, &param);
 }

 void MainModel::HandleLogout(const ::google::protobuf::Message* value) {
   XMVVM::XMVVM_ParamInt32 param;
   param.set_value(1);
   FireActionResult(Main::Action::kActionLogout, &param);
 }
