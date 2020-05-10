#include "log.h"
#include "tcp_tls_connection_impl.h"
#include <boost/asio/ssl.hpp> 
BEGIN_NAMESPACE_NET

TcpTlsConnectionImpl::TcpTlsConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request,
  NetConnection::NetConnectionDelegate* delegate,
  std::shared_ptr<BASE_LOOPER::MessagePump> pump):
  TcpConnectionBase(std::move(request), delegate, std::move(pump)),
  ssl_context_(boost::asio::ssl::context::sslv23_client) {
  boost::system::error_code ec;
  ssl_context_.set_default_verify_paths(ec);
  socket_ = std::make_unique<boost::asio::ssl::stream<boost::beast::tcp_stream>>(GetIOService(), ssl_context_);
}

boost::beast::tcp_stream& TcpTlsConnectionImpl::GetLowestLayer() {
  return boost::beast::get_lowest_layer(*socket_);
}

void TcpTlsConnectionImpl::DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
  auto self = shared_from_this();
  boost::beast::get_lowest_layer(*socket_).expires_after(std::chrono::seconds(10));
  boost::asio::async_connect(socket_->lowest_layer(), endpoints,
    [self](boost::system::error_code ec,
      boost::asio::ip::tcp::endpoint endpoint) mutable {
        //连接失败通知上层
        self->NotifyConnectComplete(ec);     
        self.reset();
    });
}

void TcpTlsConnectionImpl::DoTlsHandshake() {
  auto self = shared_from_this();
  socket_->set_verify_mode(boost::asio::ssl::verify_peer);
  socket_->set_verify_callback([this] (bool preverified, boost::asio::ssl::verify_context& ctx) ->bool {
    return VerifyCert(preverified, ctx);
    });

  socket_->async_handshake(boost::asio::ssl::stream_base::client,
    [self](boost::system::error_code ec) mutable {
      self->NotifyTlsHandshakeComplete(std::move(ec));
      self.reset();
    });
}


bool TcpTlsConnectionImpl::VerifyCert(bool preverified, boost::asio::ssl::verify_context& ctx) {
  char subject_name[256] = {0};
  /*X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
  if (nullptr != cert) {
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
    LogDebug << subject_name;
  }*/

  if (SSL_get_verify_result(socket_->native_handle()) == X509_V_OK) {
    return true;
  }
  return false;
}

void TcpTlsConnectionImpl::DoCleanUp() {
  boost::system::error_code ec;
  socket_->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  socket_->lowest_layer().close(ec);
}

END_NAMESPACE_NET



