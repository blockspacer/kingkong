#ifndef _HTTP_CLIENT_IMPL_H_
#define _HTTP_CLIENT_IMPL_H_
#include "http_connection_base.h"
#include <base/base_header.h>

BEGIN_NAMESPACE_NET

class HttpClientImpl : public HttpConnectionBase<boost::beast::tcp_stream> {
 public:
  HttpClientImpl(std::unique_ptr<HttpClientRequest> http_request,
                 std::unique_ptr<NetConnection::NetConnectionRequest> net_connection_request,
                 HttpClient::HttpClientDelegate* delegate,
                 std::shared_ptr<BASE_LOOPER::MessagePump> pump);

  void DoConnect(
      const boost::asio::ip::tcp::resolver::results_type& endpoints) override;

  void DoCleanUp() override;

 private:
};

END_NAMESPACE_NET
#endif