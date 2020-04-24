#ifndef _MODEL_H
#define _MODEL_H
#include <mvvm_base.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include <boost/any.hpp>

BEGIN_NAMESPACE_FRAME

class Model {
public:
  virtual  ~Model() = default;

  using ModelBuilder = std::function<Model* ()>;
  using SuscribeEventDelegate = std::function<void(const boost::any& value)>;

  struct SuscribeEventDelegateInfo {
    SuscribeEventDelegate delegate;
    int64_t id;

    SuscribeEventDelegateInfo(SuscribeEventDelegate delegate, int64_t id) {
      this->delegate = std::move(delegate);
      this->id = id;
    }
    SuscribeEventDelegateInfo(SuscribeEventDelegateInfo&& other) {
      delegate = std::move(other.delegate);
      id = other.id;
    }

    SuscribeEventDelegateInfo& operator=(SuscribeEventDelegateInfo&& other) {
      delegate = std::move(other.delegate);
      id = other.id;
      return *this;
    }
  };

 public:
   //工厂方法，用来创建Model
   static void RegisterModel(int32_t type, ModelBuilder builder);
   static Model* ModelOf(int32_t model_type);

   //Viewmodel 可以订阅和反订阅事件
  int64_t SubscribeActionResult(int32_t event, SuscribeEventDelegate delegate);
  void UnSubscribeActionResult(int64_t id);

  //viewmode 调用model 接口
  void Call(int32_t action, const boost::any& value);

protected:
  //model可以触发事件
  void FireActionResult(int32_t action, const boost::any& value);
  //子类实现
  virtual void HandleAction(int32_t action, const boost::any& value) {}

private:
  std::map<int32_t, std::vector<SuscribeEventDelegateInfo>> event_delegate_;

  //用来创建或者查找model
  static std::map<int32_t, ModelBuilder> model_builder_;
  static std::map<int32_t, Model*> models_;

  //事件订阅的标识
  static int64_t s_suscribe_id_;
};

END_NAMESPACE_FRAME

#endif
