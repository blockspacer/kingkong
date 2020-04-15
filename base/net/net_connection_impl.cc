﻿#include "net_connection_impl.h"
#include "log.h"
#include <boost/lexical_cast.hpp>
BEGIN_NAMESPACE_NET
#define MAX_RECV_READ 1024

NetConnectionImpl::NetConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request,
  NetConnection::NetConnectionDelegate* delegate,
                 std::shared_ptr<BASE_LOOPER::MessagePump> pump)
    : request_(std::move(request)),
      delegate_(delegate),
      pump_(std::move(pump)) {}


 NetConnectionImpl::~NetConnectionImpl() {
   if (!stoped_) {
     LogFatal << "not stoped, call DisConnect";
   }
 }

void NetConnectionImpl::Connect() {
  std::weak_ptr< NetConnectionImpl> weak_self = shared_from_this();
  pump_->PostRunable([weak_self] {
    if (auto self = weak_self.lock()) {    
        self->DoDnsResolve();
    }
  });
}

void NetConnectionImpl::DisConnect() {
  if (stoped_) {
    return;
  }
  stoped_ = true;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    delegate_ = nullptr;
  }
  //这里需要 强引用，否则不能真正 cleanup
  auto self = shared_from_this();
  pump_->PostRunable([self] {
     self->HandleCleanUp();
  });
}

void NetConnectionImpl::Send(const void* buffer, int32_t buffer_len) {
  if (stoped_) {
    return;
  }
  auto send_buffer = std::make_shared<std::string>((const char*)buffer, buffer_len);
  std::weak_ptr< NetConnectionImpl> weak_self = shared_from_this();
  pump_->PostRunable([weak_self, send_buffer] {
    if (auto self = weak_self.lock()) {
      self->DoSendFromQueue(send_buffer);
    }
  });
}

void NetConnectionImpl::OnDnsResolvered(
    const boost::asio::ip::tcp::resolver::results_type& result,
    int code,
    const std::string& msg) {

  if (nullptr != dns_resolver_) {
    dns_resolver_->Cancel();
    dns_resolver_ = nullptr;
  }

  if (0 != code) {
    HandleConnectStatus(code, msg);
    HandleCleanUp();
  } else {
    //DNS 解析完成 开始连接
    DoConnect(result);
  }
}

void NetConnectionImpl::DoDnsResolve() {
  auto dns_resolver_request =
      std::make_unique<BASE_NET::DnsResolver::DnsResolverRequest>();
  dns_resolver_request->host = request_->host;
  dns_resolver_ = BASE_NET::CreateDnsResolver(std::move(dns_resolver_request),
                                         this, pump_);
  dns_resolver_->Resolver();
}

void NetConnectionImpl::NotifyConnectComplete(const boost::system::error_code& ec) {
  HandleConnectStatus(ec.value(), ec.message());
  if (!ec) {
    switch (request_->net_type) {
    case kNetTypeTcpTls: {
      DoTlsHandshake();
    }
                       break;
    case kNetTypeWebsocket: {
      DoWebsocketHandshake();
    }
                          break;
    default:
    {
      read_buffer_ = boost::shared_array<char>(new char[MAX_RECV_READ]);
      asio_buffers_ = boost::asio::buffer(read_buffer_.get(), MAX_RECV_READ);
      DoRecvData(asio_buffers_);
    }
    break;
    }
  }
  else {
    HandleCleanUp();
  }
}

void NetConnectionImpl::NotifyHandshakeComplete(
    const boost::system::error_code& ec) {
  HandleHandshake(ec);
  if (!ec) {
    //握手成功开始接收数据
    read_buffer_ = boost::shared_array<char>(new char[MAX_RECV_READ]);
    asio_buffers_ = boost::asio::buffer(read_buffer_.get(), MAX_RECV_READ);
    DoRecvData(asio_buffers_);
  } else {
    HandleCleanUp();
  }
}

boost::asio::io_service& NetConnectionImpl::GetIOService() {
  return *(boost::asio::io_context*)pump_->Raw();
}

void NetConnectionImpl::DoSendFromQueue(std::shared_ptr<std::string> send_buffer) {
  bool write_in_progress = !send_buffers.empty();
  send_buffers.push_back(std::move(send_buffer));
  if (!write_in_progress) {    
    DoSendData(boost::asio::buffer(send_buffers.front()->c_str(),
                                   send_buffers.front()->length()));
  }
}

void NetConnectionImpl::HandleHandshake(const boost::system::error_code& ec) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (nullptr != delegate_) {
    delegate_->OnHandshake(this, ec.value(), ec.message());
  }
}

void NetConnectionImpl::DoTlsHandshake() {
  LogFatal << "Unexpected call";
}


void NetConnectionImpl::DoWebsocketHandshake() {
  LogFatal << "Unexpected call";
}

void NetConnectionImpl::NotifySendComplete(boost::system::error_code ec, std::size_t length) {
  if (ec) {
    //发生了错误，通知断开连接
    NotifyDisconnect(ec);
    HandleCleanUp();
  }
  else {
    //发送成功之后，移除队头数据，判断是否需要继续发送
    send_buffers.pop_front();
    if (!send_buffers.empty()) {
      DoSendData(boost::asio::buffer(send_buffers.front()->c_str(),
                                     send_buffers.front()->length()));
    }
  }
}

void NetConnectionImpl::NotifyRecvData(boost::system::error_code ec, std::size_t length) {
  if (ec) {
    //发生了错误，通知断开连接
    NotifyDisconnect(ec);
    HandleCleanUp();
  }
  else {
    HandleRecvData(length);
  }
}

void NetConnectionImpl::HandleCleanUp() {
  if (nullptr != dns_resolver_) {
    dns_resolver_->Cancel();
    dns_resolver_ = nullptr;
  }
  DoCleanUp();
}

void NetConnectionImpl::HandleConnectStatus(int code, const std::string& msg) {
  //失败了。通知连接失败
  std::lock_guard<std::mutex> lock(mutex_);
  if (nullptr != delegate_) {
    delegate_->OnConnect(this, code, msg);
  }
}

void NetConnectionImpl::NotifyDisconnect(boost::system::error_code& ec) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (nullptr != delegate_) {
      delegate_->OnDisconnect(this, ec.value(), ec.message());
    }
  }
}

void NetConnectionImpl::HandleRecvData(std::size_t length) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (nullptr != delegate_) {
      delegate_->OnRecvData(this, read_buffer_.get(), static_cast<int>(length));
    }
  }
  DoRecvData(asio_buffers_);
}

END_NAMESPACE_NET

