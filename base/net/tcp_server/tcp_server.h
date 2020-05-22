#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <base/base_header.h>
#include <boost/asio.hpp>

BEGIN_NAMESPACE_NET

class TcpServer {
 public:
  class TcpServerDelegate {
   public:
    virtual ~TcpServerDelegate() = default;
    virtual void OnListen(int32_t code,const std::string& msg) = 0;
    virtual void OnNewConnection(boost::asio::ip::tcp::socket socket) = 0;
  };

  struct TcpServerRequest {
    int32_t port = 0;
  };

 public:
  virtual ~TcpServer() = default;
  virtual void StartListen() = 0;
  virtual void StopListen() = 0;
};

std::shared_ptr<TcpServer> CreateTcpServer(
    std::unique_ptr<TcpServer::TcpServerRequest> request,
    TcpServer::TcpServerDelegate* delegate);

END_NAMESPACE_NET
#endif
