#include "http_client_impl.h"
#include "log.h"

BEGIN_NAMESPACE_NET
HttpClientImpl::HttpClientImpl(
    std::unique_ptr<HttpClientRequest> http_request,
    std::unique_ptr<NetConnection::NetConnectionRequest> net_connection_request,
    HttpClient::HttpClientDelegate* delegate,
    std::shared_ptr<BASE_LOOPER::MessagePump> pump)
    : HttpConnectionBase(std::move(http_request),
                         std::move(net_connection_request),
                         delegate,
                         std::move(pump)) {
  stream_ = std::make_unique<boost::beast::tcp_stream>(GetIOService());
}

void HttpClientImpl::DoConnect(
  const boost::asio::ip::tcp::resolver::results_type& endpoints) {
  stream_->expires_after(std::chrono::seconds(30));
  auto self = shared_from_this();
  stream_->async_connect(endpoints,
    [self](boost::system::error_code ec,
      boost::asio::ip::tcp::endpoint endpoint) mutable {
        self->NotifyConnectComplete(std::move(ec));
        self.reset();
    });
}

void HttpClientImpl::DoCleanUp() {
  boost::system::error_code ec;
  stream_->socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
}

END_NAMESPACE_NET
