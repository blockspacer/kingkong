#ifndef _BASE_HEADER_H_
#define _BASE_HEADER_H_
#include <namespace_define.h>
#include <string>
#include <set>
#include <vector>
#include <memory>
#include <mutex>
#include <functional>
#include <stdint.h>
#include <at_exit.h>
#include <boost/thread/once.hpp>


//是否开启assert
#ifndef NDEBUG
  #define DCHECK(con) assert(con)
#else
  #define DCHECK(con)
#endif

//构造和析构是否成对出现
#ifndef NDEBUG
#define DECLARE_OBJECT_RECORD(TypeName) \
  static std::string s_class_name_;     \
  static std::set<TypeName*> s_object_set_; \
  static std::mutex s_mutex_;

#define IMPLEMET_OBJECT_RECORD(TypeName)           \
  std::string TypeName::s_class_name_ = #TypeName; \
  std::set<TypeName*> TypeName::s_object_set_; \
  std::mutex TypeName::s_mutex_;

#define ADD_OBJECT_RECORD()                         \
  static boost::once_flag once;                                 \
  boost::call_once([] {                             \
        BASE_UTIL::AtExitManager::RegisterCallback([]{  \
              if (!s_object_set_.empty()) {             \
                  LogFatal << "class:" << s_class_name_ << " count:" << s_object_set_.size();\
              } else {\
                  LogInfo << "class:" << s_class_name_  << " ok";\
               }\
           });               \
      },                                            \
      once);                                        \
  {\
std::lock_guard<std::mutex> lock(s_mutex_);\
  s_object_set_.insert(this);\
}

#define REMOVE_OBJECT_RECORD() {\
  std::lock_guard<std::mutex> lock(s_mutex_); \
    s_object_set_.erase(this);                  \
  }

#else
  #define DECLARE_OBJECT_RECORD(TypeName)
  #define IMPLEMET_OBJECT_RECORD(TypeName)
  #define ADD_OBJECT_RECORD() 
  #define REMOVE_OBJECT_RECORD()
#endif



#ifdef WIN32
#define OS_WIN 
#elif defined(ANDROID)
#define OS_ANDROID
#else
#error "不支持的系统"
#endif // WIN32


#endif
