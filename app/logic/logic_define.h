#ifndef _APP_LOGIC_H_
#define _APP_LOGIC_H_
#include <viewmodel.h>
#include <model.h>

enum ModelType {
  kModelTypeBegin = 0,
  kModelMain = kModelTypeBegin
};


enum ViewModelType {
  kViewModelBegin = 0,
  kViewModelMain = kViewModelBegin
};

namespace Main {
  enum Properties {
    kPropertyUserName,
    kPropertyPasswd
  };

  enum Event {
    kEventLogin,
    kEventLogout
  };

  enum Action {
    kActionLogin,
    kActionLogout
  };

  struct UserNameParam {
    std::string school;
  };
};


#endif

