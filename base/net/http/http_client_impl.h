#ifndef _HTTP_CLIENT_IMPL_H_
#define _HTTP_CLIENT_IMPL_H_
#include "http_connection_base.h"
#include <base/base_header.h>

BEGIN_NAMESPACE_NET

template<class BodyType = boost::beast::http::string_body>
class HttpClientImpl : public HttpConnectionBase<boost::beast::tcp_stream, BodyType> {
 public:
  HttpClientImpl(std::unique_ptr<HttpClientRequest> http_request,
                 std::unique_ptr<NetConnection::NetConnectionRequest> net_connection_request,
                 HttpClient::HttpClientDelegate* delegate,
                 std::shared_ptr<BASE_LOOPER::MessagePump> pump)
      : HttpConnectionBase(std::move(http_request),
                           std::move(net_connection_request),
                           delegate,
                           std::move(pump)) {
    stream_ = std::make_unique<boost::beast::tcp_stream>(GetIOService());
  }

  void DoConnect(
      const boost::asio::ip::tcp::resolver::results_type& endpoints) {
    stream_->expires_after(std::chrono::seconds(30));
    auto self = shared_from_this();
    stream_->async_connect(
        endpoints, [self](boost::system::error_code ec,
                          boost::asio::ip::tcp::endpoint endpoint) mutable {
          self->NotifyConnectComplete(std::move(ec));
          self.reset();
        });
  }

  void DoCleanUp() {
    boost::system::error_code ec;
    stream_->socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  }

 private:
};

END_NAMESPACE_NET
#endif