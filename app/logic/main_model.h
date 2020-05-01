#ifndef _MAIN_MODEL_H_
#define _MAIN_MODEL_H_
#include <logic_define.h>

class MainModel :public MVVM_FRAME::Model {
public:
  MainModel();

  static Model* CreateModel() {
    return new MainModel;
  }

  void HandleAction(int32_t action, const ::google::protobuf::Message* value) override;

private:
  void HandleLogin(const ::google::protobuf::Message* value);
  void HandleLogout(const ::google::protobuf::Message* value);
};



#endif
