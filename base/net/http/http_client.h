﻿#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_
#include <base/message_loop/message_pump.h>
#include <base/base_header.h>

BEGIN_NAMESPACE_NET

class HttpClient {
public:
	virtual ~HttpClient() = default;

	class HttpClientDelegate {
	public:
		virtual void OnHttpResponse(int32_t http_code, const std::string& content) = 0;
	};

	enum HttpMethod {
		kHttpMethodGet,
		kHttpMethodPost,
		kHttpMethodPut
	};

	struct HttpClientRequest {
		std::string url;
		HttpMethod method = kHttpMethodGet;
		std::map<std::string, std::string> heads;
	};

public:
	virtual void Request() = 0;
	virtual void Cancel() = 0;
};

std::shared_ptr<HttpClient> CreateHttpClient(
    std::unique_ptr<HttpClient::HttpClientRequest> request,
		HttpClient::HttpClientDelegate* delegate,
    std::shared_ptr<BASE_LOOPER::MessagePump> pump);

END_NAMESPACE_NET


#endif // !_HTTP_CLIENT_H_
