#ifndef _BASE_WEBSOCKET_TLS_CONNECTION_IMPL_H_
#define _BASE_WEBSOCKET_TLS_CONNECTION_IMPL_H_

#include "websocket_connection_base.h"
#include <base_header.h>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/beast/ssl.hpp>

BEGIN_NAMESPACE_NET
class WebsocketTlsConnectionImpl : public WebsocketConnectionBase<boost::beast::websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream> > > {
public:
  WebsocketTlsConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request,
    NetConnection::NetConnectionDelegate* delegate,
    std::shared_ptr<BASE_LOOPER::MessagePump> pump);

protected:
  void DoTlsHandshake() override;

  void DoCleanUp() override {
    boost::system::error_code ec;
    ws_->next_layer().next_layer().close();
    ws_->close(boost::beast::websocket::close_code::normal, ec);
  }



private:
  static boost::asio::ssl::context s_ssl_context_;
};

END_NAMESPACE_NET
#endif
