#include "main_model.h"
#include "logic_define.h"

 MainModel::MainModel() {}

 void MainModel::HandleAction(int32_t action, const boost::any& value)  {
    BEGIN_HANDLE_ACTION(action)
      HANDLE_ACTION(Main::Action::kActionLogin, HandleLogin)
      HANDLE_ACTION(Main::Action::kActionLogout, HandleLogout)
    END_HANDLE_ACTION()
 }

 void MainModel::HandleLogin(const boost::any& value) {
   FireActionResult(Main::Action::kActionLogin, std::string("result"));
 }

 void MainModel::HandleLogout(const boost::any& value) {
   Main::UserNameParam param;
   param.school = "school";
   FireActionResult(Main::Action::kActionLogout, param);
 }
