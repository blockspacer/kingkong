#include "tcp_connection_impl.h"

BEGIN_NAMESPACE_NET

TcpConnectionImpl::TcpConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, 
  NetConnection::NetConnectionDelegate* delegate,
  std::shared_ptr<BASE_LOOPER::MessagePump> pump):
  TcpConnectionBase(std::move(request), delegate, std::move(pump)) {
  socket_ = std::make_unique<boost::beast::tcp_stream>(GetIOService());
}

boost::beast::tcp_stream& TcpConnectionImpl::GetLowestLayer() {
  return *socket_;
}

void TcpConnectionImpl::DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
  auto self = shared_from_this();
  socket_->async_connect(endpoints,
    [self](boost::system::error_code ec,
      boost::asio::ip::tcp::endpoint endpoint) mutable {
        self->NotifyConnectComplete(std::move(ec));
        self.reset();
    });
}


void TcpConnectionImpl::DoCleanUp() {
  boost::system::error_code ec;
    socket_->socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    socket_->socket().close(ec);
}


END_NAMESPACE_NET



