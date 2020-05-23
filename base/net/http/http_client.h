#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_
#include <base/message_loop/message_pump.h>
#include <base/base_header.h>
#include <boost/beast.hpp>
#include <unordered_map>
BEGIN_NAMESPACE_NET

class HttpClient {
public:
	virtual ~HttpClient() = default;

	class HttpClientDelegate {
	public:
		virtual void OnHttpHeads(const std::unordered_map<std::string, std::string>& heads) = 0;
		virtual void OnHttpResponse(boost::string_view content) = 0;
    	virtual void OnHttpComplete(boost::beast::http::status http_code) = 0;
	};

	enum HttpMethod {
		kHttpMethodGet,
		kHttpMethodPost,
		kHttpMethodPut
	};

	struct HttpClientRequest {
		std::string url;
		HttpMethod method = kHttpMethodGet;
		std::unordered_map<std::string, std::string> heads;
		//socks 5代理相关
    std::string proxy_host;
		uint16_t proxy_port;
		std::string proxy_username;
		std::string proxy_passwd;
	};

public:
	virtual void Request() = 0;
	virtual void Cancel() = 0;
};

std::shared_ptr<HttpClient> CreateHttpClient(
    std::unique_ptr<HttpClient::HttpClientRequest> request,
		HttpClient::HttpClientDelegate* delegate);

END_NAMESPACE_NET


#endif // !_HTTP_CLIENT_H_
