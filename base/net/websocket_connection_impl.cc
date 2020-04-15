#include "log.h"
#include "websocket_connection_impl.h"
BEGIN_NAMESPACE_NET

WebsocketConnectionImpl::WebsocketConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request,
  NetConnection::NetConnectionDelegate* delegate,
  std::shared_ptr<BASE_LOOPER::MessagePump> pump):
  NetConnectionImpl(std::move(request), delegate, std::move(pump)),
  ws_(GetIOService()) {
}

void WebsocketConnectionImpl::DoWebsocketHandshake() {
  auto self = shared_from_this();
  ws_.async_handshake(request_->host, "/", [self](boost::system::error_code& ec) {
    self->NotifyHandshakeComplete(ec);
    });
}

void WebsocketConnectionImpl::DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
  auto self = shared_from_this();
  boost::beast::get_lowest_layer(ws_).async_connect(endpoints,
    [self](boost::system::error_code& ec,
      boost::asio::ip::tcp::endpoint endpoint) {
        //连接失败通知上层
        self->NotifyConnectComplete(ec);                    
    });
}

void WebsocketConnectionImpl::DoRecvData(boost::asio::mutable_buffer& buffers) {
  auto self = shared_from_this();
  //读的时候指定最大读取就可以了
  ws_.async_read_some(buffers, [self](boost::system::error_code& ec, std::size_t length) mutable {
    self->NotifyRecvData(ec, length);
    self.reset();
    });
}

void WebsocketConnectionImpl::DoSendData(boost::asio::const_buffer& buffers) {
  auto self = shared_from_this();
  ws_.async_write(
    buffers,
    [self](boost::system::error_code& ec, std::size_t length) mutable {
      self->NotifySendComplete(ec, length);
      self.reset();
    });
}

void WebsocketConnectionImpl::DoCleanUp() {
  boost::system::error_code ec;
  ws_.close(boost::beast::websocket::close_code::normal, ec);
}

END_NAMESPACE_NET



