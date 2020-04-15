#ifndef _BASE_WEBSOCKET_CONNECTION_IMPL_H_
#define _BASE_WEBSOCKET_CONNECTION_IMPL_H_

#include "websocket_connection_base.h"
#include <base_header.h>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>

BEGIN_NAMESPACE_NET
class WebsocketConnectionImpl : public WebsocketConnectionBase<boost::beast::websocket::stream<boost::beast::tcp_stream> > {
public:
  WebsocketConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, NetConnection::NetConnectionDelegate* delegate, std::shared_ptr<BASE_LOOPER::MessagePump> pump);
};

END_NAMESPACE_NET
#endif
