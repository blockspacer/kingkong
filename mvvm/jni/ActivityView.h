#ifndef  _ANDROID_ACTIVITY_VIEW_H_
#define _ANDROID_ACTIVITY_VIEW_H_
#include <mvvm/baseview.h>

class ActivityView : public  MVVM_FRAME::BaseView {
public:
    //view model 有了改变，通知到UI
  void OnPropertyChanged(int32_t property_id, const boost::any& value) override;
  void OnEventFired(int32_t event_id, const boost::any& value) override ;

  //提供给UI 用的
  void HandleEvent(int32_t event, const boost::any& value);
};


#endif