#ifndef _MAIN_MODEL_H_
#define _MAIN_MODEL_H_
#include <logic_define.h>

class MainModel :public MVVM_FRAME::Model {
public:
  MainModel();

  static Model* CreateModel() {
    return new MainModel;
  }

  void HandleAction(int32_t action, const boost::any& value) override;

private:
  void HandleLogin(const boost::any& value);

};



#endif
