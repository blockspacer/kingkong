#ifndef _BASE_TCP_TLS_CONNECTION_IMPL_H_
#define _BASE_TCP_TLS_CONNECTION_IMPL_H_

#include <base/net/tcp/tcp_connection_base.h>
#include <base/base_header.h>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp> 
#include <boost/beast.hpp>

BEGIN_NAMESPACE_NET
class TcpTlsConnectionImpl : public TcpConnectionBase<boost::asio::ssl::stream<boost::beast::tcp_stream>> {
public:
  TcpTlsConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, NetConnection::NetConnectionDelegate* delegate, std::shared_ptr<BASE_LOOPER::MessagePump> pump);
 

protected:
  boost::beast::tcp_stream& GetLowestLayer() override;

  void DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) override;
  void DoCleanUp() override;
  void DoTlsHandshake() override;

  bool VerifyCert(bool preverified, boost::asio::ssl::verify_context& ctx);

private:
  boost::asio::ssl::context ssl_context_;
};

END_NAMESPACE_NET
#endif
