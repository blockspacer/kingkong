﻿#ifndef _BASE_NET_CONNECTION_H_
#define _BASE_NET_CONNECTION_H_
#include "message_loop/message_pump.h"
#include <base_header.h>

BEGIN_NAMESPACE_NET
class NetConnection {
public:
  virtual ~NetConnection() = default;

	struct NetConnectionRequest{
		std::string host;
		uint16_t port;
	};

	class NetConnectionDelegate {
	public:
		virtual void OnConnect(NetConnection* tcp, int code, const std::string& msg) = 0;
		virtual void OnRecvData(NetConnection* tcp, const void* buffer, int32_t buffer_len) = 0;
		virtual void OnDisconnect(NetConnection* tcp, int code, const std::string& msg) = 0;
	};


	virtual void Connect() = 0;
	virtual void DisConnect() = 0;
	virtual void Send(const void* buffer, int32_t buffer_len) = 0;
};

std::shared_ptr<NetConnection> CreateTcp(std::unique_ptr<NetConnection::NetConnectionRequest> request,
	NetConnection::NetConnectionDelegate* delegate,
	std::shared_ptr<BASE_LOOPER::MessagePump> pump);

END_NAMESPACE_NET


#endif
