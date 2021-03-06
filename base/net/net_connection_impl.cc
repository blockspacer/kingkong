﻿#include "net_connection_impl.h"
#include "log.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

BEGIN_NAMESPACE_NET
#define MAX_RECV_READ 1024

std::ostream& operator<<(std::ostream& ostream,
                         NetConnection::NetType net_type)
{
  switch (net_type) {
    case NetConnection::kNetTypeTcp: {
      return ostream << "Tcp";
    }
    case NetConnection::kNetTypeTcpTls: {
      return ostream << "TcpTLS";
    }
    case NetConnection::kNetTypeWebsocket: {
      return ostream << "TcpWebsocketTLS";
    }
    case NetConnection::kNetTypeWebsocketTls: {
      return ostream << "WebsocketTls";
    }
    case NetConnection::kNetTypeHttp: {
      return ostream << "Http";
    }
    case NetConnection::kNetTypeHttps: {
      return ostream << "Https";
    }
    default:
      break;
  }
  return ostream << net_type;
}

IMPLEMET_OBJECT_RECORD(NetConnectionImpl)

std::atomic<int32_t> NetConnectionImpl::id_(0);

NetConnectionImpl::NetConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request,
  NetConnection::NetConnectionDelegate* delegate,
                 std::shared_ptr<BASE_LOOPER::MessagePump> pump)
    : request_(std::move(request)),
      delegate_(delegate),
      pump_(std::move(pump)),
      stoped_(false) {
  ADD_OBJECT_RECORD()
  int32_t tmp_id = ++id_;
  boost::format fmt("conn:%1%");
  fmt% tmp_id;
  connection_id_ = fmt.str();
  LogDebug << "connection_id:" << connection_id_ << " host:" << request_->host << " net_type:" << request_->net_type;
}


 NetConnectionImpl::~NetConnectionImpl() {
   if (!stoped_) {
     LogFatal << "not stoped, call DisConnect";
   }
   LogDebug << "connection_id:" << connection_id_;
   REMOVE_OBJECT_RECORD()
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
  pump_->PostRunable([weak_self, send_buffer = std::move(send_buffer)] {
    if (auto self = weak_self.lock()) {
      self->DoSendFromQueue(std::move(send_buffer));
    }
  });
}

NetConnection::NetType NetConnectionImpl::net_type() {
  return request_->net_type;
}

const std::string& NetConnectionImpl::connection_id() const {
  return connection_id_;
}

void NetConnectionImpl::OnSocks5Handshake(int32_t state) {
  if (state != 0) {
    LogError << "connection_id:" << connection_id_ << " socks5 handshake state:" << state;
    HandleCleanUp();
    HandleConnectStatus(state, "socks5 handshake failed");
  } else {
    DoOtherHandshake(state, "");
  }
  socks5_client_ = nullptr;
}

void NetConnectionImpl::OnDnsResolvered(
    const boost::asio::ip::tcp::resolver::results_type& result,
    int code,
    const std::string& msg) {

  if (nullptr != dns_resolver_) {
    dns_resolver_->Cancel();
    dns_resolver_ = nullptr;
  }
  //DNS 解析失败
  if (0 != code) {
    LogError << "connection_id:" << connection_id_ << " DNS parse failed: " << msg;
    HandleConnectStatus(code, msg);
    HandleCleanUp();
  } else {
    //DNS 解析完成 开始连接
    for (auto& item : result) {
      const auto &endpoints = item.endpoint();
      LogDebug << "connection_id:" << connection_id_ << " " << endpoints;
    }
    
   
    GetLowestLayer().expires_after(std::chrono::seconds(30));
    DoConnect(result);
  }
}

void NetConnectionImpl::DoDnsResolve() {
  auto dns_resolver_request =
      std::make_unique<BASE_NET::DnsResolver::DnsResolverRequest>();
  //如果有代理，就先解析代理IP
  if (!request_->proxy_host.empty()) {
    dns_resolver_request->host = request_->proxy_host;
    dns_resolver_request->schem =
        boost::lexical_cast<std::string>(request_->proxy_port);

  } else {
    dns_resolver_request->host = request_->host;
    dns_resolver_request->schem = boost::lexical_cast<std::string>(request_->port);
  }

  dns_resolver_ = BASE_NET::CreateDnsResolver(std::move(dns_resolver_request),
                                         this, pump_);
  dns_resolver_->Resolver();
}

void NetConnectionImpl::NotifyConnectComplete(boost::system::error_code ec) {
  if (!ec) {
    //如果有代理的话，需要进行socks5 握手
    if (!request_->proxy_host.empty()) {
      Socks5Client::Socks5ProxyProperty proxy_property;
      proxy_property.server = request_->host;
      proxy_property.port = request_->port;
      proxy_property.username = request_->proxy_username;
      proxy_property.password = request_->proxy_passwd;
      socks5_client_ = CreateSocks5Client(GetLowestLayer(),
          proxy_property,
          this);
      socks5_client_->Connect();
    } else {
      DoOtherHandshake(ec.value(), ec.message());
    }
  }
  else {
    LogError << "connection_id:" << connection_id_ << " connect failed: " << ec.message();
    HandleCleanUp();
    HandleConnectStatus(ec.value(), ec.message());
  }
}

void NetConnectionImpl::NotifyTlsHandshakeComplete(boost::system::error_code ec) {
  if (!ec) {
    switch (request_->net_type) {
    case kNetTypeWebsocketTls: {
      DoWebsocketHandshake();
    }
                             break;
    default: {
      //TLS 握手成功开始接收数据
      HandleConnectStatus(ec.value(), ec.message());
      StartRecvDate();
    }
           break;
    }
  
  } else {
    LogError << "connection_id:" << connection_id_ << " tls handshake failed: " << ec.message();
    HandleConnectStatus(ec.value(), ec.message());
    HandleCleanUp();
  }
}

void NetConnectionImpl::NotifyWebsocketHandshakeComplte(boost::system::error_code ec) {
  HandleConnectStatus(ec.value(), ec.message());
  if (!ec) {
    //Websocket 握手成功。 这是最后一步
    StartRecvDate();
  }
  else {
    LogError << "connection_id:" << connection_id_ << " websocket handshake failed: " << ec.message();
    HandleCleanUp();
  }
}

boost::asio::io_service& NetConnectionImpl::GetIOService() {
  return *(boost::asio::io_context*)pump_->Raw();
}

void NetConnectionImpl::DoSendFromQueue(std::shared_ptr<std::string> send_buffer) {
  bool write_in_progress = !send_buffers.empty();
  send_buffers.push_back(std::move(send_buffer));
  if (!write_in_progress && !already_clean_up) {
    DoSendData(boost::asio::buffer(send_buffers.front()->c_str(),
                                   send_buffers.front()->length()));
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
    LogError << "connection_id:" << connection_id_ << " send failed: " << ec.message();
    NotifyDisconnect(ec);
    HandleCleanUp();
  }
  else {
    //发送成功之后，移除队头数据，判断是否需要继续发送
    send_buffers.pop_front();
    if (!send_buffers.empty() && !already_clean_up) {
      DoSendData(boost::asio::buffer(send_buffers.front()->c_str(),
                                     send_buffers.front()->length()));
    }
  }
}

void NetConnectionImpl::NotifyRecvData(boost::system::error_code ec, std::size_t length) {
  if (ec) {
    //发生了错误，通知断开连接
    LogError << "connection_id:" << connection_id_ << " recv failed: " << ec.message();
    NotifyDisconnect(ec);
    HandleCleanUp();
  }
  else {
    HandleRecvData(length);
  }
}

void NetConnectionImpl::HandleCleanUp() {
  if (already_clean_up) {
    return;
  }
  already_clean_up = true;
  if (nullptr != socks5_client_) {
    socks5_client_ = nullptr;
  }
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

void NetConnectionImpl::StartRecvDate() {
  read_buffer_ = boost::shared_array<char>(new char[MAX_RECV_READ]);
  asio_buffers_ = boost::asio::buffer(read_buffer_.get(), MAX_RECV_READ);
  GetLowestLayer().expires_never();
  DoRecvData(asio_buffers_);
}

void NetConnectionImpl::DoOtherHandshake(int32_t code, const std::string& msg) {
  switch (request_->net_type) {
    case kNetTypeWebsocketTls:
    case kNetTypeTcpTls:
    case kNetTypeHttps: {
      DoTlsHandshake();
    } break;
    case kNetTypeWebsocket: {
      DoWebsocketHandshake();
    } break;
    default: {
      //普通的连接直接完成
      HandleConnectStatus(code, msg);
      StartRecvDate();
    } break;
  }
}

END_NAMESPACE_NET


