#ifndef _VIEW_MODEL_H
#define _VIEW_MODEL_H
#include <mvvm/mvvm_base.h>
#include <mvvm/model.h>
#include <boost/thread/once.hpp>

BEGIN_NAMESPACE_FRAME

//属性和动作分开
//属性专门用来设置，更新UI 控件的内容。 动作控件的一些行为 点击，选择
//属性的内容使用结构体，Variant 这些虽然灵活，但是非常容易出错，并且完全不知道里面的内容，需要有文档记录Variant 字段。存进去很方便，但是读取
//却麻烦容易出错。boost::any 内部是一个 VMParamBase 或者子类，使用的时候强转成真实类型再来操作

class ViewModel : public std::enable_shared_from_this<ViewModel> {
public:
  ViewModel(int32_t viewmode_type);
  virtual ~ViewModel();

  using ViewModelBuilder = std::function<std::shared_ptr<ViewModel>()>;
  using PropertyChangedDelegate = std::function<void(int32_t property_id, const ::google::protobuf::Message* value)>;
  using EventFiredDelegate = std::function<void(int32_t event_id, const ::google::protobuf::Message* value)>;

  //VM 有数据改变的时候会回调给UI
  void BindPropertyChanged(PropertyChangedDelegate delegate);
  //绑定事件
  void BindEvent(EventFiredDelegate event_fired_delegate);

  //UI 层有动作的时候
  void HandleEvent(int32_t event, const ::google::protobuf::Message* value);


  int32_t viewmode_type();

  //上层绑定和解绑之后通知
  void NotifyBind(void* context);
  void NotifyUnBind();

  //工厂方法，用来创建VM
  static void RegisterViewModel(int32_t type, ViewModelBuilder builder);
  static std::shared_ptr<ViewModel> Create(int32_t type);

protected:
  //VM 需要实现这个方法，用来初始化UI，或者监听一些Model数据变化
  virtual void OnAttach() = 0;
  virtual void OnDetach() = 0;

  //子类实现，如果有action 需要处理
  virtual void OnEventFired(int32_t event, const ::google::protobuf::Message* value) {}
  
  //提供给子类的帮助函数
  void FireProperty(int32_t property_id, const ::google::protobuf::Message* value);

  void* context() {
    return context_;
  }

protected:
  std::map<int64_t, int32_t> subscribe_ids_;

private:
  void UnSubscrieAllAction();

  PropertyChangedDelegate property_changed_delegate_;
  EventFiredDelegate event_fired_delegate_;

  int32_t viewmode_type_ = -1;

  static std::map< int32_t, ViewModelBuilder> vm_builder_;
  void* context_ = nullptr;
};

END_NAMESPACE_FRAME

#endif
