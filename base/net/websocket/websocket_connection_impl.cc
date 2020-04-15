#include "log.h"
#include "websocket_connection_impl.h"
BEGIN_NAMESPACE_NET

WebsocketConnectionImpl::WebsocketConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request,
  NetConnection::NetConnectionDelegate* delegate,
  std::shared_ptr<BASE_LOOPER::MessagePump> pump):
  WebsocketConnectionBase(std::move(request), delegate, std::move(pump)) {
  ws_ = std::make_unique<boost::beast::websocket::stream<boost::beast::tcp_stream>>(GetIOService());
}

END_NAMESPACE_NET



