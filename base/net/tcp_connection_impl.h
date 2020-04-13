#ifndef _BASE_TCP_CONNECTION_IMPL_H_
#define _BASE_TCP_CONNECTION_IMPL_H_

#include "net_connection_impl.h"
#include <base_header.h>
#include <boost/asio.hpp>

BEGIN_NAMESPACE_NET
class TcpConnectionImpl : public NetConnectionImpl {
public:
  TcpConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, NetConnection::NetConnectionDelegate* delegate, std::shared_ptr<BASE_LOOPER::MessagePump> pump);
 

protected:
  void DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) override;
  void DoRecvData(boost::asio::mutable_buffer& buffers) override;
  void DoSendData(boost::asio::const_buffer& buffers) override;
  void DoCleanUp() override;

private:
  boost::asio::ip::tcp::socket socket_;
};

END_NAMESPACE_NET
#endif
