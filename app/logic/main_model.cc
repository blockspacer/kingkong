#include "main_model.h"

 MainModel::MainModel() {}

 void MainModel::HandleAction(int32_t action, const boost::any& value)  {
    BEGIN_HANDLE_ACTION(action)
      HANDLE_ACTION(Main::Action::kActionLogin, HandleLogin)
    END_HANDLE_ACTION()
 }

 void MainModel::HandleLogin(const boost::any& value) {
   FireActionResult(Main::Action::kActionLogin, std::string("result"));
 }
