#include "net_connection.h"
#include "tcp/tcp_connection_impl.h"
#include "tcp/tcp_tls_connection_impl.h"
#include "websocket/websocket_connection_impl.h"
#include "websocket/websocket_tls_connection_impl.h"
#include "http/http_client_impl.h"
#include "http/https_client_impl.h"
#include "http/http_parser.h"
#include "http/http_downloader_impl.h"
#include <boost/asio/ssl.hpp> 

//http://blog.sina.com.cn/s/blog_541348370101czw8.html

BEGIN_NAMESPACE_NET
std::shared_ptr<NetConnection> CreateTcp(
  std::unique_ptr<NetConnection::NetConnectionRequest> request,
  NetConnection::NetConnectionDelegate* delegate,
  std::shared_ptr<BASE_LOOPER::MessagePump> pump) {
  switch (request->net_type) {
  case BASE_NET::NetConnection::kNetTypeTcp: {
      return std::make_shared<BASE_NET::TcpConnectionImpl>(
        std::move(request), delegate, std::move(pump));
    }
  case BASE_NET::NetConnection::kNetTypeTcpTls: {
      return std::make_shared<BASE_NET::TcpTlsConnectionImpl>(
        std::move(request), delegate, std::move(pump));
    }
  case BASE_NET::NetConnection::kNetTypeWebsocket: {
      return std::make_shared<BASE_NET::WebsocketConnectionImpl>(
        std::move(request), delegate, std::move(pump));
    }
  case BASE_NET::NetConnection::kNetTypeWebsocketTls: {
    return std::make_shared<BASE_NET::WebsocketTlsConnectionImpl>(
      std::move(request), delegate, std::move(pump));
  }
  }
  return nullptr;
}



std::shared_ptr<HttpClient> CreateHttpClient(
  std::unique_ptr<HttpClient::HttpClientRequest> request,
  HttpClient::HttpClientDelegate* delegate,
  std::shared_ptr<BASE_LOOPER::MessagePump> pump) {
  auto net_connection_request = std::make_unique<NetConnection::NetConnectionRequest>();
  //开始解析URL
  http_parser_url parse_url;
  http_parser_url_init(&parse_url);
  int code = http_parser_parse_url(request->url.c_str(), request->url.length(), false, &parse_url);
  if (0 == code) {
    net_connection_request->port = parse_url.port;
    std::string scheme = request->url.substr(parse_url.field_data[UF_SCHEMA].off, parse_url.field_data[UF_SCHEMA].len);
    if (scheme == "https") {
      net_connection_request->net_type = NetConnection::kNetTypeHttps;
      if (net_connection_request->port == 0) {
        net_connection_request->port = 443;
      }
    }
    else {
      net_connection_request->net_type = NetConnection::kNetTypeHttp;
      if (net_connection_request->port == 0) {
        net_connection_request->port = 80;
      }
    }
    net_connection_request->host = request->url.substr(parse_url.field_data[UF_HOST].off, parse_url.field_data[UF_HOST].len);
    net_connection_request->path = request->url.substr(parse_url.field_data[UF_PATH].off, parse_url.field_data[UF_PATH].len);
    if (net_connection_request->path.empty()) {
      net_connection_request->path = "/";
    }
  }
  net_connection_request->proxy_host = request->proxy_host;
  net_connection_request->proxy_port = request->proxy_port;
  net_connection_request->proxy_username = request->proxy_username;
  net_connection_request->proxy_passwd = request->proxy_passwd;
  if (net_connection_request->net_type == NetConnection::kNetTypeHttp) {
    return std::make_shared<HttpClientImpl>(std::move(request),
                                            std::move(net_connection_request),
                                            delegate, std::move(pump));
  } else {
    return std::make_shared<HttpsClientImpl>(std::move(request),
                                            std::move(net_connection_request),
                                            delegate, std::move(pump));
  }
}



std::shared_ptr<HttpDownloader> CreateHttpDownloader(
    std::unique_ptr<HttpDownloader::HttpDownloaderRequest> request,
    HttpDownloader::HttpDownloaderDelegate* delegate,
    std::shared_ptr<BASE_LOOPER::MessagePump> pump) {
  return std::make_shared<HttpDownloaderImpl>(std::move(request), delegate, std::move(pump));
}
END_NAMESPACE_NET
