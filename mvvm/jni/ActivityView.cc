#include <base/jni_util/var_cache.h>
#include <base/jni_util/scope_jenv.h>
#include <base/jni_util/comm_function.h>
#include <base/log.h>
#include "ActivityView.h"

DEFINE_FIND_CLASS(KC2Java, "com/mvvm/MVVMHelp")

DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyEventFiredBuffer, KC2Java, "NotifyEventFired", "(JI[B)V")
static void NotifyEventFired(int64_t instance, int event_id, const std::string& value) {
  VarCache* cache_instance = VarCache::Singleton();
  ScopeJEnv scope_jenv(cache_instance->GetJvm());
  JNIEnv *env = scope_jenv.GetEnv();
  jbyteArray j_value = JNU_Buffer2JbyteArray(env, value.c_str(), value.length());
  JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyEventFiredBuffer, instance, event_id, j_value);
  JNU_FreeJbyteArray(env, j_value);
}

DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyPropertyChanged, KC2Java, "NotifyPropertyChanged", "(JI[B)V")
static void NotifyPropertyChanged(int64_t instance, int event_id, const std::string& new_buffer) {
    LogDebug << "instance:"<<instance<<"  event_id:"<<event_id << "  buffer:" << new_buffer.length();
    VarCache* cache_instance = VarCache::Singleton();
    ScopeJEnv scope_jenv(cache_instance->GetJvm());
    JNIEnv *env = scope_jenv.GetEnv();
    jbyteArray j_new_value = JNU_Buffer2JbyteArray(env, new_buffer.c_str(), new_buffer.length());
    JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyPropertyChanged, instance, event_id, j_new_value);
    JNU_FreeJbyteArray(env, j_new_value);
}


void ActivityView::OnPropertyChanged(int32_t property_id, const ::google::protobuf::Message* value) {
    std::string buffer;
    if (nullptr != value) {
      value->SerializeToString(&buffer);
    }
    NotifyPropertyChanged((int64_t)this, property_id, buffer);
}

void ActivityView::OnEventFired(int32_t event_id, const ::google::protobuf::Message* value) {
    std::string buffer;
  if (nullptr != value) {
    value->SerializeToString(&buffer);
  }
  NotifyEventFired((int64_t)this, event_id, buffer);
}

void ActivityView::HandleEvent(int32_t event, const ::google::protobuf::Message* value) {
  vm_->HandleEvent(event, value);
}