#include "log.h"
#include "tcp_tls_connection_impl.h"
#include <boost/asio/ssl.hpp> 
BEGIN_NAMESPACE_NET

TcpTlsConnectionImpl::TcpTlsConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request,
  NetConnection::NetConnectionDelegate* delegate,
  std::shared_ptr<BASE_LOOPER::MessagePump> pump):
  NetConnectionImpl(std::move(request), delegate, std::move(pump)),
  ssl_context_(boost::asio::ssl::context::sslv23_client),
  socket_(GetIOService(), ssl_context_) {
  boost::system::error_code ec;
  ssl_context_.set_default_verify_paths(ec);
}

void TcpTlsConnectionImpl::DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
  auto self = shared_from_this();
  boost::asio::async_connect(socket_.lowest_layer(), endpoints,
    [self](boost::system::error_code ec,
      boost::asio::ip::tcp::endpoint endpoint) {
        //连接失败通知上层
        self->NotifyConnectComplete(ec);                    
    });
}

void TcpTlsConnectionImpl::DoTlsHandshake() {
  auto self = shared_from_this();
  socket_.set_verify_mode(boost::asio::ssl::verify_peer);
  socket_.set_verify_callback([this] (bool preverified, boost::asio::ssl::verify_context& ctx) ->bool {
    return VerifyCert(preverified, ctx);
    });

  socket_.async_handshake(boost::asio::ssl::stream_base::client,
    [self](const boost::system::error_code& error) {
      self->NotifyHandshakeComplete(error);
    });
}


bool TcpTlsConnectionImpl::VerifyCert(bool preverified, boost::asio::ssl::verify_context& ctx) {
  char subject_name[256] = {0};
  X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
  if (nullptr != cert) {
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
    LogDebug << subject_name;
  }

  if (SSL_get_verify_result(socket_.native_handle()) == X509_V_OK) {
    return true;
  }
  return false;
}

void TcpTlsConnectionImpl::DoRecvData(boost::asio::mutable_buffer& buffers) {
  auto self = shared_from_this();
  //读的时候指定最大读取就可以了
  socket_.async_read_some(buffers, [self](boost::system::error_code ec, std::size_t length) mutable {
    self->NotifyRecvData(ec, length);
    self.reset();
    });
}

void TcpTlsConnectionImpl::DoSendData(boost::asio::const_buffer& buffers) {
  auto self = shared_from_this();
  boost::asio::async_write(
    socket_,
    buffers,
    [self](boost::system::error_code ec, std::size_t length) {
      self->NotifySendComplete(ec, length);
    });
}

void TcpTlsConnectionImpl::DoCleanUp() {
  boost::system::error_code ec;
  socket_.lowest_layer().cancel(ec);
  socket_.lowest_layer().close(ec);
}

END_NAMESPACE_NET



