#ifndef _MAIN_MODEL_H_
#define _MAIN_MODEL_H_
#include <logic_define.h>

class MainModel :public MVVM_FRAME::Model {
public:
  MainModel();

  static Model* CreateModel() {
    return new MainModel;
  }

  void HandleEvent(int32_t event, const ::google::protobuf::Message* value, ::google::protobuf::Message* result) override;

private:
  void HandleLogin(const ::google::protobuf::Message* value);
  void HandleLogout(const ::google::protobuf::Message* value);
};



#endif
