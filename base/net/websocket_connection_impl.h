#ifndef _BASE_WEBSOCKET_CONNECTION_IMPL_H_
#define _BASE_WEBSOCKET_CONNECTION_IMPL_H_

#include "net_connection_impl.h"
#include <base_header.h>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>

BEGIN_NAMESPACE_NET
class WebsocketConnectionImpl : public NetConnectionImpl {
public:
  WebsocketConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, NetConnection::NetConnectionDelegate* delegate, std::shared_ptr<BASE_LOOPER::MessagePump> pump);

protected:
  void DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) override;
  void DoRecvData(boost::asio::mutable_buffer& buffers) override;
  void DoSendData(boost::asio::const_buffer& buffers) override;
  void DoCleanUp() override;
  void DoWebsocketHandshake() override;

private:
  boost::beast::websocket::stream<boost::beast::tcp_stream> ws_;
};

END_NAMESPACE_NET
#endif
