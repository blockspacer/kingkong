#ifndef _BASE_TCP_CONNECTION_IMPL_H_
#define _BASE_TCP_CONNECTION_IMPL_H_

#include <base/net/tcp_client/tcp_connection_base.h>
#include <base/base_header.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

BEGIN_NAMESPACE_NET
class TcpConnectionImpl : public TcpConnectionBase<boost::beast::tcp_stream> {
public:
  TcpConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, NetConnection::NetConnectionDelegate* delegate, std::shared_ptr<BASE_LOOPER::MessagePump> pump);
 

protected:
  boost::beast::tcp_stream& GetLowestLayer();
  void DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) override;
  void DoCleanUp() override;

private:
};

END_NAMESPACE_NET
#endif
