#include "tcp_server_impl.h"
#include <base/message_loop/message_loop.h>
#include "base/log.h"
BEGIN_NAMESPACE_NET

TcpServerImpl::TcpServerImpl(
    std::unique_ptr<TcpServer::TcpServerRequest> request,
    TcpServer::TcpServerDelegate* delegate)
    : request_(std::move(request)),
      acceptor_(*(boost::asio::io_context*)BASE_LOOPER::MessageLoop::IOMessagePump()->Raw(),
          boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                         request_->port)),
      delegate_(delegate){}


void TcpServerImpl::StartListen() { 
  auto self = shared_from_this();
  acceptor_.async_accept([self](boost::system::error_code ec,
                                boost::asio::ip::tcp::socket socket) mutable {
    self->NotifyClient(std::move(ec), std::move(socket));
    self.reset();
  });
}

void TcpServerImpl::Cleanup() {
  boost::system::error_code ec;
  acceptor_.cancel(ec);
  acceptor_.close(ec);
}

void TcpServerImpl::NotifyClient(boost::system::error_code ec,
                                 boost::asio::ip::tcp::socket socket) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (nullptr != delegate_) {
      if (ec) {
        LogError << ec.message();
        delegate_->OnListen(ec.value(), ec.message());
      } else {
        delegate_->OnNewConnection(std::move(socket));
      }
    }  
  }
}

void TcpServerImpl::StopListen() {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    delegate_ = nullptr;
  }
  auto self = shared_from_this();
  BASE_LOOPER::MessageLoop::IOMessagePump()->PostRunable([self]() { self->Cleanup();
  });
}

std::shared_ptr<TcpServer> CreateTcpServer(
    std::unique_ptr<TcpServer::TcpServerRequest> request,
    TcpServer::TcpServerDelegate* delegate) {
  return std::make_shared<TcpServerImpl>(std::move(request), delegate);
}
END_NAMESPACE_NET
