#include <base/jni_util/var_cache.h>
#include <base/jni_util/scope_jenv.h>
#include <base/jni_util/comm_function.h>
#include <base/log.h>
#include "ActivityView.h"

DEFINE_FIND_CLASS(KC2Java, "com/mvvm/MVVMHelp")


DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyEventFiredString, KC2Java, "NotifyEventFired", "(JILjava/lang/String;)V")
static void NotifyEventFired(int64_t instance, int event_id, const std::string& value) {
    LogDebug << "instance:"<<instance<<"  event_id:"<< event_id << "  value:" << value;
  VarCache* cache_instance = VarCache::Singleton();
  ScopeJEnv scope_jenv(cache_instance->GetJvm());
  JNIEnv *env = scope_jenv.GetEnv();
  jstring j_value = JNU_Chars2Jstring(env, value.c_str());
  JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyEventFiredString, instance, event_id, j_value);
  env->DeleteLocalRef(j_value);
}

DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyEventFiredInt, KC2Java, "NotifyEventFired", "(JII)V")
static void NotifyEventFired(int64_t instance, int event_id, int32_t value) {
    LogDebug << "instance:"<<instance<<"  event_id:"<<event_id << "  value:" << value;
  VarCache* cache_instance = VarCache::Singleton();
  ScopeJEnv scope_jenv(cache_instance->GetJvm());
  JNIEnv *env = scope_jenv.GetEnv();
  JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyEventFiredInt, instance, event_id, value);
}

DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyEventFiredBool, KC2Java, "NotifyEventFired", "(JIZ)V")
static void NotifyEventFired(int64_t instance, int event_id, bool value) {
    LogDebug << "instance:"<<instance<<"  event_id:"<<event_id << "  value:" << value;
  VarCache* cache_instance = VarCache::Singleton();
  ScopeJEnv scope_jenv(cache_instance->GetJvm());
  JNIEnv *env = scope_jenv.GetEnv();
  JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyEventFiredBool, instance, event_id, value);
}

DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyEventFiredBuffer, KC2Java, "NotifyEventFired", "(JI[B)V")
static void NotifyEventFired(int64_t instance, int event_id, const char* buffer, int32_t len) {
  VarCache* cache_instance = VarCache::Singleton();
  ScopeJEnv scope_jenv(cache_instance->GetJvm());
  JNIEnv *env = scope_jenv.GetEnv();
  jbyteArray j_value = JNU_Buffer2JbyteArray(env, buffer, len);
  JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyEventFiredBuffer, instance, event_id, j_value);
  JNU_FreeJbyteArray(env, j_value);
}

DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyPropertyChangedBuffer, KC2Java, "NotifyPropertyChanged", "(JI[B)V")
static void NotifyPropertyChangedBuffer(int64_t instance, int event_id, const char* new_buffer, int32_t new_len) {
    LogDebug << "instance:"<<instance<<"  event_id:"<<event_id << "  buffer:" << new_len;
    VarCache* cache_instance = VarCache::Singleton();
  ScopeJEnv scope_jenv(cache_instance->GetJvm());
  JNIEnv *env = scope_jenv.GetEnv();
  jbyteArray j_new_value = JNU_Buffer2JbyteArray(env, new_buffer, new_len);
  JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyPropertyChangedBuffer, instance, event_id, j_new_value);
  JNU_FreeJbyteArray(env, j_new_value);
}

DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyPropertyChangedString, KC2Java, "NotifyPropertyChanged", "(JILjava/lang/String;)V")
static void NotifyPropertyChangedString(int64_t instance, int event_id, const std::string& new_buffer) {
    LogDebug << "instance:"<<instance<<"  event_id:"<<event_id << "  value:" << new_buffer;
  VarCache* cache_instance = VarCache::Singleton();
  ScopeJEnv scope_jenv(cache_instance->GetJvm());
  JNIEnv *env = scope_jenv.GetEnv();

  jstring j_new_value = JNU_Chars2Jstring(env, new_buffer.c_str());
  JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyPropertyChangedString, instance, event_id, j_new_value);
  env->DeleteLocalRef(j_new_value);
}

DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyPropertyChangedInt, KC2Java, "NotifyPropertyChanged", "(JII)V")
static void NotifyPropertyChangedInt(int64_t instance, int event_id, int32_t new_buffer) {
    LogDebug << "instance:"<<instance<<"  event_id:"<<event_id << "  value:" << new_buffer;
    VarCache* cache_instance = VarCache::Singleton();
  ScopeJEnv scope_jenv(cache_instance->GetJvm());
  JNIEnv *env = scope_jenv.GetEnv();

  JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyPropertyChangedInt, instance, event_id, new_buffer);
}

DEFINE_FIND_STATIC_METHOD(KC2Java_NotifyPropertyChangedBool, KC2Java, "NotifyPropertyChanged", "(JIZ)V")
static void NotifyPropertyChangedBool(int64_t instance, int event_id, bool new_buffer) {
    LogDebug << "instance:"<<instance<<"  event_id:"<<event_id << "  value:" << new_buffer;
  VarCache* cache_instance = VarCache::Singleton();
  ScopeJEnv scope_jenv(cache_instance->GetJvm());
  JNIEnv *env = scope_jenv.GetEnv();

  JNU_CallStaticMethodByMethodInfo(env,KC2Java_NotifyPropertyChangedBool, instance, event_id, new_buffer);
}


void ActivityView::OnPropertyChanged(int32_t property_id, const boost::any& value) {
  {
      auto real_value = boost::any_cast<std::string>(&value);
    if (nullptr != real_value) {
      NotifyPropertyChangedString((int64_t)this, property_id, *real_value);
      return;
    }
  }
    {
        auto real_value = boost::any_cast<int32_t >(&value);
        if (nullptr != real_value) {
            NotifyPropertyChangedInt((int64_t)this, property_id, *real_value);
            return;
        }
    }
}

void ActivityView::OnEventFired(int32_t event_id, const boost::any& value) {
  {
    auto int_value = boost::any_cast<std::string>(&value);
    if (nullptr != int_value) {
      NotifyEventFired((int64_t)this, event_id, *int_value);
      return;
    }
  }

  {
    auto int_value = boost::any_cast<int32_t>(&value);
    if (nullptr != int_value) {
      NotifyEventFired((int64_t)this, event_id, *int_value);
      return;
    }
  }

  {
    auto int_value = boost::any_cast<bool >(&value);
    if (nullptr != int_value) {
      NotifyEventFired((int64_t)this, event_id, *int_value);
      return;
    }
  }
  {
   //自定义结构体，自己解析
  }
}

void ActivityView::HandleEvent(int32_t event, const boost::any& value) {
  vm_->HandleEvent(event, value);
}