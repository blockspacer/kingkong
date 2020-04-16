﻿#ifndef _BASE_TCP_TLS_CONNECTION_IMPL_H_
#define _BASE_TCP_TLS_CONNECTION_IMPL_H_

#include "tcp_connection_base.h"
#include <base_header.h>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp> 

BEGIN_NAMESPACE_NET
class TcpTlsConnectionImpl : public TcpConnectionBase<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> {
public:
  TcpTlsConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, NetConnection::NetConnectionDelegate* delegate, std::shared_ptr<BASE_LOOPER::MessagePump> pump);
 

protected:
  void DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) override;
  void DoCleanUp() override;
  void DoTlsHandshake() override;

  bool VerifyCert(bool preverified, boost::asio::ssl::verify_context& ctx);

private:
  boost::asio::ssl::context ssl_context_;
};

END_NAMESPACE_NET
#endif