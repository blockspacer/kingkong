#include "tcp_impl.h"
#include "log.h"
#include <boost/lexical_cast.hpp>
BEGIN_NAMESPACE_NET
#define MAX_RECV_READ 1024

TcpImpl::TcpImpl(std::unique_ptr<Tcp::TcpRequest> request,
                 Tcp::TcpDelegate* delegate,
                 std::shared_ptr<BASE_LOOPER::MessagePump> pump)
    : request_(std::move(request)),
      delegate_(delegate),
      pump_(std::move(pump)),
      socket_(*(boost::asio::io_context*)pump_->Raw()){}


 TcpImpl::~TcpImpl() {
   if (!stoped_) {
     LogFatal << "not stoped, call DisConnect";
   }
 }

void TcpImpl::Connect() {
  std::weak_ptr< TcpImpl> weak_self = shared_from_this();
  pump_->PostRunable([weak_self] {
    if (auto self = weak_self.lock()) {    
        self->DoDnsResolve();
    }
  });
}

void TcpImpl::DisConnect() {
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
     self->DoCleanUp();
  });
}

void TcpImpl::Send(const void* buffer, int32_t buffer_len) {
  if (stoped_) {
    return;
  }
  auto send_buffer = std::make_shared<std::string>((const char*)buffer, buffer_len);
  std::weak_ptr< TcpImpl> weak_self = shared_from_this();
  pump_->PostRunable([weak_self, send_buffer] {
    if (auto self = weak_self.lock()) {
      self->DoSend(send_buffer);
    }
  });
}


void TcpImpl::OnDnsResolvered(
    const boost::asio::ip::tcp::resolver::results_type& result,
    int code,
    const std::string& msg) {

  if (nullptr != dns_resolver_) {
    dns_resolver_->Cancel();
    dns_resolver_ = nullptr;
  }

  if (0 != code) {
    NotifyConnectStatus(code, msg);
    DoCleanUp();
  } else {
    //DNS 解析完成 开始连接
    DoConnect(result);
  }
}

void TcpImpl::DoDnsResolve() {
  auto dns_resolver_request =
      std::make_unique<BASE_NET::DnsResolver::DnsResolverRequest>();
  dns_resolver_request->host = request_->host;
  dns_resolver_ = BASE_NET::CreateDnsResolver(std::move(dns_resolver_request),
                                         this, pump_);
  dns_resolver_->Resolver();
}

void TcpImpl::DoConnect(
    const boost::asio::ip::tcp::resolver::results_type& endpoints) {
  auto self = shared_from_this();
  boost::asio::async_connect(socket_, endpoints,
                             [self](boost::system::error_code ec,
                                    boost::asio::ip::tcp::endpoint endpoint) {
                               self->NotifyConnectStatus(ec.value(),
                                                         ec.message());
                               if (!ec) {
                                 self->DoRecvData();
                               } else {
                                 self->DoCleanUp();
                               }
                             });
}

void TcpImpl::DoRecvData() {
  if (nullptr == read_buffer_) {
    read_buffer_ = boost::shared_array<char>(new char[MAX_RECV_READ]);
  }

  auto self = shared_from_this();
  //读的时候指定最大读取就可以了
  socket_.async_read_some(boost::asio::buffer(read_buffer_.get(), MAX_RECV_READ),
      [self](boost::system::error_code ec, std::size_t length) mutable{
        if (ec) {
          //发生了错误，通知断开连接
          self->NotifyDisconnect(ec);
          self->DoCleanUp();
        } else {
          self->NotifyRecvData(length);
        }
        self.reset();
      });
}

void TcpImpl::DoSend(std::shared_ptr<std::string> send_buffer) {
  bool write_in_progress = !send_buffers.empty();
  send_buffers.push_back(std::move(send_buffer));
  if (!write_in_progress) {    
    AsyncSend();
  }
}


void TcpImpl::AsyncSend() {
  auto self = shared_from_this();
  boost::asio::async_write(socket_,
      boost::asio::buffer(send_buffers.front()->c_str(),
                          send_buffers.front()->length()),
      [self](boost::system::error_code ec, std::size_t /*length*/) {
        if (ec) {
          //发生了错误，通知断开连接
          self->NotifyDisconnect(ec);
          self->DoCleanUp();
        } else {
          //发送成功之后，移除队头数据，判断是否需要继续发送
          self->send_buffers.pop_front();
          if (!self->send_buffers.empty()) {
            self->AsyncSend();
          }
        }
    });
}

void TcpImpl::DoCleanUp() {
  if (nullptr != dns_resolver_) {
    dns_resolver_->Cancel();
    dns_resolver_ = nullptr;
  }

  boost::system::error_code ec;
  socket_.cancel(ec);
  socket_.close(ec);
}

void TcpImpl::NotifyConnectStatus(int code, const std::string& msg) {
  //失败了。通知连接失败
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (nullptr != delegate_) {
      delegate_->OnConnect(this, code, msg);
    }
  }
}


void TcpImpl::NotifyDisconnect(boost::system::error_code& ec) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (nullptr != delegate_) {
      delegate_->OnDisconnect(this, ec.value(), ec.message());
    }
  }
}

void TcpImpl::NotifyRecvData(std::size_t length) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (nullptr != delegate_) {
      delegate_->OnRecvData(this, read_buffer_.get(), static_cast<int>(length));
    }
  }
  DoRecvData();
}

std::shared_ptr<Tcp> CreateTcp(std::unique_ptr<Tcp::TcpRequest> request,
                               Tcp::TcpDelegate* delegate,
                                std::shared_ptr<BASE_LOOPER::MessagePump> pump) {
  return std::make_shared<TcpImpl>(std::move(request), delegate, std::move(pump));
}
END_NAMESPACE_NET

