#ifndef _APP_LOGIC_H_
#define _APP_LOGIC_H_
#include <mvvm/viewmodel.h>
#include <mvvm/model.h>
#include <mvvm/pb/MvvmBasicParam.pb.h>

namespace VMDefine {
  enum ModelType {
    kModelMain = __LINE__,
    kModelProxy,
    kModelSocks5Server,
    kModelSelectApp
  };


  enum ViewModelType {
    kViewModelMain = __LINE__,
    kViewModelProxy,
    kViewModelSocks5Server,
    kViewModelSelectApp
  };
};

namespace Main {
  enum Event {
    kEventLogin = __LINE__,
    kEventLogout
  };

  enum Properties {
    kPropertyUserName = __LINE__,
    kPropertyPasswd
  };
};
#endif

