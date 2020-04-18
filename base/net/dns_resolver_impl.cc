#include "message_loop/message_pump.h"
#include "log.h"
#include "dns_resolver_impl.h"

BEGIN_NAMESPACE_NET

IMPLEMET_OBJECT_RECORD(DnsResolverImpl)

DnsResolverImpl::DnsResolverImpl(
    std::unique_ptr<DnsResolver::DnsResolverRequest> request,
    DnsResolver::DnsResolverDelegate* delegate,
    std::shared_ptr<BASE_LOOPER::MessagePump> pump)
    : request_(std::move(request)),
  delegate_(delegate),
  pump_(std::move(pump)),
  resolver_(*(boost::asio::io_context*)pump_->Raw()),
  stoped_(false) {
  ADD_OBJECT_RECORD();
}

 DnsResolverImpl::~DnsResolverImpl() {
   if (!stoped_) {
     LogFatal << "Dns Resolver not stoped";
   }
   REMOVE_OBJECT_RECORD()
 }

void DnsResolverImpl::Resolver() {
   if (stoped_) {
     return;
   }
  std::weak_ptr<DnsResolverImpl> weak_self = shared_from_this();
  pump_->PostRunable([weak_self] {
    if (auto self = weak_self.lock()) {
      self->DoResolver();
    }
 });
}


void DnsResolverImpl::Cancel() {
  if (stoped_) {
    return;
  }
  stoped_ = true;
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    delegate_ = nullptr;
  }
  //需要捕获强引用，否则Cancel 不掉
  auto self = shared_from_this();
  pump_->PostRunable([self] () mutable {   
      self->DoCancel();
      self.reset();
 });
}

void DnsResolverImpl::DoResolver() {
  auto self = shared_from_this();
  auto& io_service = *(boost::asio::io_context*)pump_->Raw();
  resolver_.async_resolve(request_->host, request_->schem,
    [self](boost::system::error_code ec,
      const boost::asio::ip::tcp::resolver::results_type& endpoints) mutable {
        self->NotifyResult(std::move(ec), endpoints);
        //避免 循环引用导致的内存无法释放，这里需要 手动reset
        self.reset();
    });
}


void DnsResolverImpl::DoCancel() {
  resolver_.cancel();
}

void DnsResolverImpl::NotifyResult(
    boost::system::error_code err,
    const boost::asio::ip::tcp::resolver::results_type& endpoints) {
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (nullptr != delegate_) {
      delegate_->OnDnsResolvered(endpoints, err.value(), err.message());
    }
  }
  resolver_.cancel();
}

std::shared_ptr<DnsResolver> CreateDnsResolver(
    std::unique_ptr<DnsResolver::DnsResolverRequest> request,
    DnsResolver::DnsResolverDelegate* delegate,
    std::shared_ptr<BASE_LOOPER::MessagePump> pump){
  return std::make_shared<DnsResolverImpl>(std::move(request), delegate, std::move(pump));
} 


END_NAMESPACE_NET


