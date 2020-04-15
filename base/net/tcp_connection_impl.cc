#include "tcp_connection_impl.h"

BEGIN_NAMESPACE_NET

TcpConnectionImpl::TcpConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, 
  NetConnection::NetConnectionDelegate* delegate,
  std::shared_ptr<BASE_LOOPER::MessagePump> pump):
  NetConnectionImpl(std::move(request), delegate, std::move(pump)),
  socket_(GetIOService()) {

}

void TcpConnectionImpl::DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
  auto self = shared_from_this();
  boost::asio::async_connect(socket_, endpoints,
    [self](boost::system::error_code ec,
      boost::asio::ip::tcp::endpoint endpoint) {
        self->NotifyConnectComplete(ec);
    });
}

void TcpConnectionImpl::DoRecvData(boost::asio::mutable_buffer& buffers) {
  auto self = shared_from_this();
  //读的时候指定最大读取就可以了
  socket_.async_read_some(buffers, [self](boost::system::error_code ec, std::size_t length) mutable {
        self->NotifyRecvData(ec, length);
        self.reset();
      });
}

void TcpConnectionImpl::DoSendData(boost::asio::const_buffer& buffers) {
  auto self = shared_from_this();
  boost::asio::async_write(
      socket_,
      buffers,
      [self](boost::system::error_code ec, std::size_t length) {
        self->NotifySendComplete(ec, length);
      });
}

void TcpConnectionImpl::DoCleanUp() {
  boost::system::error_code ec;
  socket_.cancel(ec);
  socket_.close(ec);
}


END_NAMESPACE_NET



