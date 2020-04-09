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


#ifdef WIN32
#define OS_WIN 
#elif defined(ANDROID)
#define OS_ANDROID
#else
#error "不支持的系统"
#endif // WIN32


#endif
