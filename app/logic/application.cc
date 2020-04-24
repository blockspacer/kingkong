#include "application.h"
#include "main_viewmodel.h"
#include "main_model.h"

void Application::Init() {
  MVVM_FRAME::ViewModel::RegisterViewModel(kViewModelMain, MainViewModel::CreateViewModel);
  MVVM_FRAME::Model::RegisterModel(kModelMain, MainModel::CreateModel);
}
