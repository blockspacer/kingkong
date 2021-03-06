﻿#ifndef _THREAD_UTIL_H_
#define _THREAD_UTIL_H_
#include <base/base_header.h>

BEGIN_NAMESPACE_THREAD
//设置线程的名字
void SetCurrentThreadName(const std::string& thread_name);

//获取线程的名字
std::string GetCurrentThreadName();

//获取线程id
std::string GetCurrentThreadId();

//线程TLS
void SetThreadTls(void* value);
void* GetThreadTls();


END_NAMESPACE_THREAD
#endif
