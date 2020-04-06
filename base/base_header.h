#ifndef _BASE_HEADER_H_
#define _BASE_HEADER_H_
#include <namespace_define.h>
#include <stdint.h>



#ifdef WIN32
#define OS_WIN 
#elif defined(ANDROID)
#define OS_ANDROID
#else
#error "不支持的系统"
#endif // WIN32


#endif
