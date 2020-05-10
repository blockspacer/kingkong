#include "socks5_client.h"
#include <base/log.h>
#include <boost/shared_array.hpp>
BEGIN_NAMESPACE_NET

enum Socks5CheckStatus {
  kProxySocks5Init = 0,
  kProxySocks5Hello,
  kProxySocks5NeedAuth,
  kProxySocks5AuthSuccess,
  kProxySocks5AuthReq,
  kProxySocks5ConSvr,
  kProxySocks5ConSvrSuccess,
};

static const uint8_t kSocks5Version = 0x05;
static const uint8_t kSocks5AuthVersion = 0x01;
static const uint8_t kSockstNAuthMethos = 0x01;
static const uint8_t kSocks5NoAuth = 0x00;
static const uint8_t kSocks5UsrNamePwdAuth = 0x02;
static const uint8_t kSocks5RespStatus = 0x00;
static const uint8_t kSocks5ConnectCmd = 0x01;
static const uint8_t kSocks5RsvVal = 0x00;
static const uint8_t kSocksATYPIPv4 = 0x01;
static const uint8_t kSocksATYPHost = 0x03;
static const uint8_t kSocksATYPIPv6 = 0x04;


class Socks5ClientImpl :
  public Socks5Client,
  public std::enable_shared_from_this< Socks5ClientImpl> {
public:
  explicit Socks5ClientImpl(boost::beast::tcp_stream& stream,
    const Socks5ProxyProperty& proxy_info,
    Socks5HandshakeDelegate* delegate);

  ~Socks5ClientImpl() override;

protected:
  void Connect() override;

private:
  void DoReceiveData();
  void DoSendData(const uint8_t* buffer, int32_t len);

  bool LoopSocks5Handshake(uint8_t* data, size_t len);
  void DoSendHello();
  void HandleHello(uint8_t* data, size_t len);

  void DoSocks5Auth();
  void HandleSocks5Auth(uint8_t* data, size_t len);

  void DoConnectMyServer();
  void HandleConnectBussinessServer(uint8_t* data, size_t len);

private:
  Socks5HandshakeDelegate* delegate_ = nullptr;
  std::string write_data_;
  //用来保存读取到的数据
  boost::shared_array<char> read_buffer_;
  //一个封装类，给asio 读取数据用
  boost::asio::mutable_buffer asio_buffers_;
  Socks5CheckStatus socks5_status_;
  std::string received_data_;
  boost::beast::tcp_stream& stream_;
  Socks5ProxyProperty proxy_info_;
};

Socks5ClientImpl::Socks5ClientImpl(boost::beast::tcp_stream& stream, 
  const Socks5ProxyProperty& proxy_info,
  Socks5HandshakeDelegate* delegate) :
  socks5_status_(kProxySocks5Init),
  stream_(stream),
  proxy_info_(proxy_info),
  delegate_(delegate) {
}

Socks5ClientImpl::~Socks5ClientImpl() {
}

void Socks5ClientImpl::DoReceiveData() {
  std::weak_ptr<Socks5ClientImpl> weak_self = shared_from_this();
  //开始读取消息
  stream_.async_read_some(asio_buffers_, [weak_self, this](boost::system::error_code ec, std::size_t length) {
    if (auto self = weak_self.lock()) {
          if (ec) {
            //失败了
            LogError << "read_some:" << ec.message();
            delegate_->OnSocks5Handshake(-1);
          } else {
            LoopSocks5Handshake((uint8_t*)read_buffer_.get(), length);
          }
    }
  });
}

void Socks5ClientImpl::DoSendData(const uint8_t* buffer, int32_t len) {
  write_data_ = std::string((const char*)buffer, len);
  std::weak_ptr<Socks5ClientImpl> weak_self = shared_from_this();
  boost::asio::async_write(stream_, boost::asio::buffer(write_data_.c_str(), write_data_.length()),
      [weak_self, this](boost::system::error_code ec, std::size_t length) {
        if (auto self = weak_self.lock()) {
          if (ec) {
            //失败了
            LogError << "async_write:" << ec;
            delegate_->OnSocks5Handshake(-1);
          }
        }
      });
}

void Socks5ClientImpl::Connect() {
  if (kProxySocks5Init != socks5_status_) {
    LogError << "check status:" << socks5_status_;
    return;
  }
  read_buffer_ = boost::shared_array<char>(new char[10]);
  asio_buffers_ = boost::asio::buffer(read_buffer_.get(), 10);
  LoopSocks5Handshake(nullptr, 0);
}

bool Socks5ClientImpl::LoopSocks5Handshake(uint8_t* data, size_t len) {
  switch (socks5_status_) {
  case BASE_NET::kProxySocks5Init:
    DoSendHello();
    break;
  case BASE_NET::kProxySocks5Hello:
    HandleHello(data, len);
    break;
  case BASE_NET::kProxySocks5NeedAuth:
    DoSocks5Auth();
    break;
  case BASE_NET::kProxySocks5AuthSuccess:
    DoConnectMyServer();
    break;
  case BASE_NET::kProxySocks5AuthReq:
    HandleSocks5Auth(data, len);
    break;
  case BASE_NET::kProxySocks5ConSvr:
    HandleConnectBussinessServer(data, len);
    break;
  default:
    break;
  }
  return true;
}

void Socks5ClientImpl::DoSendHello() {
  uint8_t socks5_greet_data[] = { kSocks5Version, kSockstNAuthMethos,
    (proxy_info_.username.empty() || proxy_info_.password.empty()) ? kSocks5NoAuth : kSocks5UsrNamePwdAuth };
  DoSendData(socks5_greet_data, sizeof(socks5_greet_data) / sizeof(uint8_t));
  socks5_status_ = kProxySocks5Hello;
  DoReceiveData();
}

void Socks5ClientImpl::HandleHello(uint8_t* data, size_t len) {
  received_data_.append(reinterpret_cast<char*>(data), len);
  if (received_data_.size() < 2) {
    LogInfo << "proxy response continue:" << received_data_.size();
    DoReceiveData();
    return;
  }
  uint8_t ver = received_data_[0];
  uint8_t method = received_data_[1];
  if (kSocks5Version != ver) {
    LogError << "socks5 hello proxy ver:" << ver;
    delegate_->OnSocks5Handshake(-1);
    return;
  }
  if (kSocks5NoAuth == method) {   //NO AUTHENTICATION REQUIRED
    socks5_status_ = kProxySocks5AuthSuccess;
  } else if (kSocks5UsrNamePwdAuth == method) { //USERNAME/PASSWORD
    socks5_status_ = kProxySocks5NeedAuth;
  } else {
    LogError << "unsupport enc method:" << method;
    delegate_->OnSocks5Handshake(-1);
    return;
  }
  received_data_.clear();
  LoopSocks5Handshake(nullptr, 0);
}

void Socks5ClientImpl::DoSocks5Auth() {
  std::vector<char> buf;
  uint8_t socks5_auth_head[] = { kSocks5AuthVersion };
  buf.assign(socks5_auth_head, socks5_auth_head + sizeof(socks5_auth_head) / sizeof(uint8_t));
  //username
  uint8_t len = static_cast<uint8_t>(proxy_info_.username.length());
  buf.push_back(len);
  buf.insert(buf.end(), (uint8_t*)proxy_info_.username.c_str(), (uint8_t*)proxy_info_.username.c_str() + len); //NOLINT
  //passwd
  len = static_cast<uint8_t>(proxy_info_.password.length());
  buf.push_back(len);
  buf.insert(buf.end(), (uint8_t*)proxy_info_.password.c_str(), (uint8_t*)proxy_info_.password.c_str() + len);//NOLINT

  DoSendData((const uint8_t*)&buf[0], buf.size());
  socks5_status_ = kProxySocks5AuthReq;
  DoReceiveData();
}

void Socks5ClientImpl::HandleSocks5Auth(uint8_t* data, size_t len) {
  received_data_.append(reinterpret_cast<char*>(data), len);
  if (received_data_.length() < 2) {
    DoReceiveData();
    LogInfo << "proxy response continue:" << received_data_.size();
    return;
  }
  uint8_t ver = received_data_[0];
  uint8_t status = received_data_[1];
  if (kSocks5AuthVersion != ver || kSocks5RespStatus != status) {
    LogError << "socks5 auth ver:" << ver << "status:" << status;
    delegate_->OnSocks5Handshake(-1);
    return;
  }
  received_data_.clear();
  socks5_status_ = kProxySocks5AuthSuccess;
  LoopSocks5Handshake(nullptr, 0);
}

void Socks5ClientImpl::DoConnectMyServer() {
  uint8_t socks_type = kSocksATYPHost;
  //判断是否是IPV4
  boost::system::error_code ec;
  boost::asio::ip::address_v4 v4;
  boost::asio::ip::address_v6 v6;
  v4.from_string(proxy_info_.server, ec);
  if (!ec) {
    socks_type = kSocksATYPIPv4;
  } else {    
    v6.from_string(proxy_info_.server, ec);
    if (!ec) {
      socks_type = kSocksATYPIPv6;
    }
  }

  uint8_t head_data[] = { kSocks5Version, kSocks5ConnectCmd, kSocks5RsvVal, socks_type };
  std::vector<char> buf;
  buf.assign(head_data, head_data + sizeof(head_data) / sizeof(uint8_t));
  switch (socks_type) {
  case kSocksATYPIPv4:
  {
    auto net_addr = v4.to_bytes();
    buf.insert(buf.end(), (uint8_t*)&net_addr[0], (uint8_t*)&net_addr[0] + net_addr.size());//NOLINT

    uint16_t net_port = htons(proxy_info_.port);
    buf.insert(buf.end(), (uint8_t*)&net_port, (uint8_t*)&net_port + sizeof(net_port));//NOLINT
  }
  break;
  case kSocksATYPIPv6:
  {
    auto net_addr = v6.to_bytes();
    buf.insert(buf.end(), (uint8_t*)&net_addr[0], (uint8_t*)&net_addr[0] + net_addr.size());//NOLINT

    uint16_t net_port = htons(proxy_info_.port);
    buf.insert(buf.end(), (uint8_t*)&net_port, (uint8_t*)&net_port + sizeof(net_port));//NOLINT
  }
  break;
  case kSocksATYPHost:
  {
    uint8_t server_len = static_cast<uint8_t>(proxy_info_.server.length());
    buf.push_back(server_len);
    buf.insert(buf.end(), (uint8_t*)proxy_info_.server.c_str(), (uint8_t*)proxy_info_.server.c_str() + server_len);//NOLINT

    uint16_t net_port = htons(proxy_info_.port);
    buf.insert(buf.end(), (uint8_t*)&net_port, (uint8_t*)&net_port + sizeof(net_port));//NOLINT
  }
  break;
  default:
    break;
  }

  DoSendData((const uint8_t*)&buf[0], buf.size());
  socks5_status_ = kProxySocks5ConSvr;
  DoReceiveData();
}

void Socks5ClientImpl::HandleConnectBussinessServer(uint8_t* data, size_t len) {
  received_data_.append(reinterpret_cast<char*>(data), len);
  if (received_data_.length() < 4) {
    DoReceiveData();
    LogInfo << "proxy response continue:" << received_data_.size();
    return;
  }
  uint8_t atyp = received_data_[3];
  // head(4) + ipv4(4) + port(2)
  if ((kSocksATYPIPv4 == atyp) && (received_data_.length() < 4 + 4 + 2)) {
    return;
  }
  // head(4) + len(1) + host + port(2)
  if ((kSocksATYPHost == atyp) && (received_data_.length() < (size_t)(4 + 1 + received_data_[4] + 2))) {
    return;
  }
  // head(4) + ipv6(16) + port(2)
  if ((kSocksATYPIPv6 == atyp) && (received_data_.length() < 4 + 16 + 2)) {
    return;
  }
  uint8_t ver = received_data_[0];
  uint8_t status = received_data_[1];

  if (kSocks5Version != ver || kSocks5RespStatus != status) {
    LogError << "connect my server auth ver:" << ver << "status:" << status;
    delegate_->OnSocks5Handshake(-1);
    return;
  }
  socks5_status_ = kProxySocks5ConSvrSuccess;
  delegate_->OnSocks5Handshake(0);
}

std::shared_ptr<Socks5Client> CreateSocks5Client(
    boost::beast::tcp_stream& stream,
    const Socks5Client::Socks5ProxyProperty& proxy_info,
    Socks5Client::Socks5HandshakeDelegate* delegate) {
  return std::make_unique<Socks5ClientImpl>(stream, proxy_info, delegate);
}

END_NAMESPACE_NET
