﻿#ifndef _BASE_DNS_RESOLVER_IMPL_H_
#define _BASE_DNS_RESOLVER_IMPL_H_
#include "message_loop/message_pump.h"
#include "dns_resolver.h"
#include <base_header.h>

BEGIN_NAMESPACE_NET
class DnsResolverImpl :public DnsResolver, public std::enable_shared_from_this<DnsResolverImpl> {
public:
  DnsResolverImpl(std::unique_ptr<DnsResolver::DnsResolverRequest> request,
                  DnsResolver::DnsResolverDelegate* delegate,
                  std::shared_ptr<BASE_LOOPER::MessagePump> pump);

  ~DnsResolverImpl();

	void Resolver() override;
  void Cancel() override;


private:
  void DoResolver();
  void DoCancel();

 void NotifyResult(const boost::system::error_code& err,
     const boost::asio::ip::tcp::resolver::results_type& endpoints);


private:
  std::unique_ptr<DnsResolver::DnsResolverRequest> request_;
  std::recursive_mutex mutex_;
  DnsResolver::DnsResolverDelegate* delegate_ = nullptr;
  std::shared_ptr<BASE_LOOPER::MessagePump> pump_;
  boost::asio::ip::tcp::resolver resolver_;
  std::atomic_bool stoped_ = false;
};

END_NAMESPACE_NET


#endif
