#ifndef _BASE_DNS_RESOLVER_H_
#define _BASE_DNS_RESOLVER_H_
#include "message_loop/message_pump.h"
#include <base_header.h>
#include <boost/asio.hpp>

BEGIN_NAMESPACE_NET
class DnsResolver {
public:
  virtual ~DnsResolver() = default;

	struct DnsResolverRequest{
		std::string host;
	};

	class DnsResolverDelegate {
	public:
		virtual void OnDnsResolvered(const boost::asio::ip::tcp::resolver::results_type& result, int code, const std::string& msg) = 0;
	};

	virtual void Resolver() = 0;
	virtual void Cancel() = 0;
};

std::shared_ptr<DnsResolver> CreateDnsResolver(std::unique_ptr<DnsResolver::DnsResolverRequest> request,
	DnsResolver::DnsResolverDelegate* delegate,
	std::shared_ptr<BASE_LOOPER::MessagePump> pump);

END_NAMESPACE_NET



#endif
