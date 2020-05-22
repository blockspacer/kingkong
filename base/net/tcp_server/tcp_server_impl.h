#ifndef _TCP_SERVER_IMPL_H_
#define _TCP_SERVER_IMPL_H_
#include "tcp_server.h"
#include <base/base_header.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

BEGIN_NAMESPACE_NET
class TcpServerImpl : public TcpServer,
                      public std::enable_shared_from_this<TcpServerImpl> {
 public:
  TcpServerImpl(std::unique_ptr<TcpServer::TcpServerRequest> request,
                TcpServer::TcpServerDelegate* delegate);

  void StartListen() override;

  void StopListen() override;

private:
  void Cleanup();

  void NotifyClient(boost::system::error_code ec,
                    boost::asio::ip::tcp::socket socket);

  std::unique_ptr<TcpServer::TcpServerRequest> request_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::mutex mutex_;
  TcpServer::TcpServerDelegate* delegate_;
};


END_NAMESPACE_NET
#endif
