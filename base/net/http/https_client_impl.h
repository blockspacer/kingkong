#ifndef _HTTPS_CLIENT_IMPL_H_
#define _HTTPS_CLIENT_IMPL_H_
#include "http_connection_base.h"
#include <base/base_header.h>
#include <boost/beast/ssl.hpp>

BEGIN_NAMESPACE_NET

class HttpsClientImpl : public HttpConnectionBase<boost::beast::ssl_stream<boost::beast::tcp_stream>> {
 public:
   HttpsClientImpl(std::unique_ptr<HttpClientRequest> http_request,
                 std::unique_ptr<NetConnection::NetConnectionRequest> net_connection_request,
                 HttpClient::HttpClientDelegate* delegate,
                 std::shared_ptr<BASE_LOOPER::MessagePump> pump);

  void DoConnect(
      const boost::asio::ip::tcp::resolver::results_type& endpoints) override;

  void DoCleanUp() override;

  void DoTlsHandshake() override;
 private:
   boost::asio::ssl::context ssl_context_;
};

END_NAMESPACE_NET
#endif