#ifndef _BASE_NET_CONNECTION_H_
#define _BASE_NET_CONNECTION_H_
#include <base/message_loop/message_pump.h>
#include <base/base_header.h>

BEGIN_NAMESPACE_NET
class NetConnection {
public:
  virtual ~NetConnection() = default;

	enum NetType {
		kNetTypeTcp = 1,
		kNetTypeTcpTls,
		kNetTypeWebsocket,
		kNetTypeWebsocketTls,
		kNetTypeHttp,
		kNetTypeHttps,
	};

	struct NetConnectionRequest{
		std::string host;
		uint16_t port;
		//是否支持tls
		NetType net_type = kNetTypeTcp;
		//http，或者websocket 的paht
		std::string path = "/";
		//socks5 代理参数
		std::string proxy_host;
		uint16_t proxy_port;
		std::string proxy_username;
		std::string proxy_passwd;
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
	virtual NetType net_type() = 0;
	virtual const std::string& connection_id() const = 0;
};

std::ostream& operator<<(std::ostream& ostream,
												 NetConnection::NetType net_type); 

std::shared_ptr<NetConnection> CreateTcpClient(std::unique_ptr<NetConnection::NetConnectionRequest> request,
	NetConnection::NetConnectionDelegate* delegate);


std::shared_ptr<NetConnection> CreateWebsocket(
    std::unique_ptr<NetConnection::NetConnectionRequest> request,
    NetConnection::NetConnectionDelegate* delegate);

END_NAMESPACE_NET

#endif
