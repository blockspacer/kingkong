#include "net_connection.h"
#include "tcp/tcp_connection_impl.h"
#include "tcp/tcp_tls_connection_impl.h"
#include "websocket/websocket_connection_impl.h"
#include "websocket/websocket_tls_connection_impl.h"
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

END_NAMESPACE_NET
