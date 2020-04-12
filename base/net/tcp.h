#ifndef _BASE_TCP_H_
#define _BASE_TCP_H_
#include "message_loop/message_pump.h"
#include <base_header.h>

BEGIN_NAMESPACE_NET
class Tcp {
public:
  virtual ~Tcp() = default;

	struct TcpRequest{
		std::string host;
		uint16_t port;
	};

	class TcpDelegate {
	public:
		virtual void OnConnect(Tcp* tcp, int code, const std::string& msg) = 0;
		virtual void OnRecvData(Tcp* tcp, const void* buffer, int32_t buffer_len) = 0;
		virtual void OnDisconnect(Tcp* tcp, int code, const std::string& msg) = 0;
	};


	virtual void Connect() = 0;
	virtual void DisConnect() = 0;
	virtual void Send(const void* buffer, int32_t buffer_len) = 0;
};

std::shared_ptr<Tcp> CreateTcp(std::unique_ptr<Tcp::TcpRequest> request,
	Tcp::TcpDelegate* delegate,
	std::shared_ptr<BASE_LOOPER::MessagePump> pump);

END_NAMESPACE_NET


#endif
