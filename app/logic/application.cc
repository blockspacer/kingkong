#include "application.h"
#include "main_viewmodel.h"
#include "main_model.h"
#include <base/log.h>
#include <boost/filesystem.hpp>
#include <base/message_loop/message_loop.h>
void Application::Init(const std::string& data_dir) {
  boost::filesystem::path data_dir_path(data_dir);
  boost::filesystem::path log_dir = data_dir_path / "log";
  BASE_LOG::InitLog(BASE_LOG::kLogLevelDebug, boost::filesystem::system_complete(log_dir).string());
  BASE_LOOPER::MessageLoop::InitMessageLoop();

  MVVM_FRAME::ViewModel::RegisterViewModel(VMDefine::kViewModelMain, MainViewModel::CreateViewModel);
  MVVM_FRAME::Model::RegisterModel(VMDefine::kModelMain, MainModel::CreateModel);
}

void Application::UnInit() {
  BASE_LOOPER::MessageLoop::UnintMessageLoop();
  BASE_LOG::UnInitLog();
}
