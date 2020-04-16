#include "tcp_connection_impl.h"

BEGIN_NAMESPACE_NET

TcpConnectionImpl::TcpConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, 
  NetConnection::NetConnectionDelegate* delegate,
  std::shared_ptr<BASE_LOOPER::MessagePump> pump):
  TcpConnectionBase(std::move(request), delegate, std::move(pump)) {
  socket_ = std::make_unique<boost::asio::ip::tcp::socket>(GetIOService());
}

void TcpConnectionImpl::DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
  auto self = shared_from_this();
  boost::asio::async_connect(*socket_, endpoints,
    [self](boost::system::error_code ec,
      boost::asio::ip::tcp::endpoint endpoint) {
        self->NotifyConnectComplete(std::move(ec));
    });
}


void TcpConnectionImpl::DoCleanUp() {
  boost::system::error_code ec;
  socket_->cancel(ec);
  socket_->close(ec);
}


END_NAMESPACE_NET



