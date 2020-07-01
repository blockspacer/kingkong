#ifndef _AUTH_UTIL_H_
#define _AUTH_UTIL_H_
#include <base/base_header.h>

BEGIN_NAMESPACE_AUTH

using AuthResponseDelegate = std::function<void(int status,const std::string& desc)>;
//开始认证请求
int32_t AuthRequest(const std::string& url, int32_t product_type, int32_t version, const std::string& token, AuthResponseDelegate delegate);
//取消认证请求
void AuthCancel(int32_t id);

END_NAMESPACE_AUTH
#endif
