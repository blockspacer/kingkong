#include "https_client_impl.h"
#include "log.h"

BEGIN_NAMESPACE_NET
HttpsClientImpl::HttpsClientImpl(
    std::unique_ptr<HttpClientRequest> http_request,
    std::unique_ptr<NetConnection::NetConnectionRequest> net_connection_request,
    HttpClient::HttpClientDelegate* delegate,
    std::shared_ptr<BASE_LOOPER::MessagePump> pump)
    : HttpConnectionBase(std::move(http_request),
                         std::move(net_connection_request),
                         delegate,
                         std::move(pump)),
  ssl_context_(boost::asio::ssl::context::sslv23_client) {
  boost::system::error_code ec;
  ssl_context_.set_default_verify_paths(ec);
  stream_ = std::make_unique<boost::beast::ssl_stream<boost::beast::tcp_stream>>(GetIOService(), ssl_context_);
}

void HttpsClientImpl::DoConnect(
  const boost::asio::ip::tcp::resolver::results_type& endpoints) {

  if (!SSL_set_tlsext_host_name(stream_->native_handle(), request_->host.c_str())) {
    boost::beast::error_code ec{
      static_cast<int>(::ERR_get_error()),
      boost::asio::error::get_ssl_category()
    };
    LogError << ec.message();
    NotifyConnectComplete(std::move(ec));
    return;
  }

  boost::beast::get_lowest_layer(*stream_).expires_after(std::chrono::seconds(30));
  auto self = shared_from_this();
  boost::beast::get_lowest_layer(*stream_).async_connect(endpoints,
    [self](boost::system::error_code ec,
      boost::asio::ip::tcp::endpoint endpoint) mutable {
        self->NotifyConnectComplete(std::move(ec));
        self.reset();
    });
}

void HttpsClientImpl::DoTlsHandshake() {
  auto self = shared_from_this();
  stream_->async_handshake(boost::asio::ssl::stream_base::client,
                           [self](boost::system::error_code ec) mutable {
                             self->NotifyTlsHandshakeComplete(std::move(ec));
                             self.reset();
                           });
}

void HttpsClientImpl::DoCleanUp() {
  boost::system::error_code ec;
  boost::beast::get_lowest_layer(*stream_).socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
}

END_NAMESPACE_NET

