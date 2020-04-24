#ifndef _TIME_UTIL_H_
#define _TIME_UTIL_H_
#include <base/base_header.h>
#include <chrono>
#include <ctime>

BEGIN_NAMESPACE_TIME

//获取当前时间戳，单位(秒)
uint64_t GetTimestamp();
// 获取当前时间戳，单位毫秒
uint64_t GetTimestampMsec();
//获取当前时间
std::tm MakeTime(uint64_t seconds);
//获取当前时间tickcount，内部抽象了一个开始时间，返回值是一个uint64. 防止外部使用 chrono 对象，过于复杂 单位毫秒
uint64_t GetTickCount2();

END_NAMESPACE_TIME
#endif
