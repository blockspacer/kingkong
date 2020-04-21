#ifndef _MVVM_BASE_H
#define _MVVM_BASE_H
#include <string>

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE() }

#define MMV_FRAME mvvm::framework
#define BEGIN_NAMESPACE_FRAME BEGIN_NAMESPACE(mvvm) BEGIN_NAMESPACE(framework)
#define END_NAMESPACE_FRAME END_NAMESPACE() END_NAMESPACE()



BEGIN_NAMESPACE_FRAME
//为什么不用联合体，联合体需要有一个参数指定类型，获取值得时候不方便。
struct VMParamBase {
  explicit VMParamBase(const char* value) { str_value = value; }

  explicit VMParamBase(const std::string& value) { str_value = value; }

  explicit VMParamBase(bool value) { bool_value = value; }

  explicit VMParamBase(int32_t value) { int32_value = value; }

  explicit VMParamBase(int64_t value) { int64_value = value; }

  explicit VMParamBase(uint64_t value) { uint64_value = value; }

  virtual ~VMParamBase() = default;

  std::string str_value;
  bool bool_value;
  int32_t int32_value;
  int64_t int64_value;
  uint64_t uint64_value;
};
END_NAMESPACE_FRAME

#endif