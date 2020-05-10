#ifndef BASE_NET_SOCKS5_IMPL_H_
#define BASE_NET_SOCKS5_IMPL_H_
#include <base/base_header.h>
#include <boost/beast.hpp>

BEGIN_NAMESPACE_NET

class Socks5Client {
public:
  struct Socks5ProxyProperty {
    std::string server;
    uint16_t port = 0;
    std::string username;
    std::string password;
  };

  class Socks5HandshakeDelegate {
  public:
    virtual ~Socks5HandshakeDelegate() = default;
    virtual void OnSocks5Handshake(int32_t state) = 0;
  };

public:
  virtual ~Socks5Client() = default;

  virtual void Connect() = 0;
};

std::shared_ptr<Socks5Client> CreateSocks5Client(
    boost::beast::tcp_stream& stream,
    const Socks5Client::Socks5ProxyProperty& proxy_info,
    Socks5Client::Socks5HandshakeDelegate* delegate);


END_NAMESPACE_NET

#endif // !BASE_NET_TCP_CONNECTION_IMPL_H_
