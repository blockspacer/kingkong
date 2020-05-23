#ifndef _BASE_NET_CONNECTION_IMPL_H_
#define _BASE_NET_CONNECTION_IMPL_H_

#include <base/net/dns_resolver.h>
#include <base/net/net_connection.h>
#include <base/base_header.h>
#include <boost/shared_array.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include "socks5_client.h"
#include <queue>
#include <atomic>

BEGIN_NAMESPACE_NET
//所有的网络操作都需要抛到pump 线程执行
class NetConnectionImpl : public NetConnection,
  public DnsResolver::DnsResolverDelegate,
  public Socks5Client::Socks5HandshakeDelegate,
  public std::enable_shared_from_this<NetConnectionImpl> {
public:
  NetConnectionImpl(std::unique_ptr<NetConnection::NetConnectionRequest> request, NetConnection::NetConnectionDelegate* delegate, std::shared_ptr<BASE_LOOPER::MessagePump> pump);
  ~NetConnectionImpl();


  void Connect() override;
  void DisConnect() override;

  void Send(const void* buffer, int32_t buffer_len) override;
  NetConnection::NetType net_type() override;

  const std::string& connection_id() const override;

public:
  //不能的模块用来实现不同的连接
  virtual void DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) = 0;
  //具体协议实现接收数据
  virtual void DoRecvData(boost::asio::mutable_buffer buffers) = 0;
  //协议发送数据
  virtual void DoSendData(boost::asio::const_buffer buffers) = 0;
  //协议清理资源
  virtual void DoCleanUp() = 0;
  //https tcp tls 需要的握手
  virtual void DoTlsHandshake();
  //websocket 需要的握手
  virtual void DoWebsocketHandshake();

  //SOCKS5 握手的时候需要拿到底层指针进行握手
  virtual  boost::beast::tcp_stream& GetLowestLayer() = 0;

  //通用的一些接口，具体的业务完成之后用来通知
  void NotifySendComplete(boost::system::error_code ec, std::size_t length);
  void NotifyRecvData(boost::system::error_code ec, std::size_t length);
  void NotifyConnectComplete(boost::system::error_code ec);
  void NotifyTlsHandshakeComplete(boost::system::error_code ec);
  void NotifyWebsocketHandshakeComplte(boost::system::error_code ec);

  boost::asio::io_service& GetIOService();

private:
  void OnSocks5Handshake(int32_t state) override;

  void OnDnsResolvered(const boost::asio::ip::tcp::resolver::results_type& result, int code, const std::string& msg) override;

  //都是在同一个pump 线程执行
  void DoDnsResolve();
  
  void DoSendFromQueue(std::shared_ptr<std::string> send_buffer);
  void HandleCleanUp();
  //回调连接状态
  void HandleConnectStatus(int code, const std::string& msg);
  //断开连接的回调
  void NotifyDisconnect(boost::system::error_code& ec);
  //回调接收到了数据
  void HandleRecvData(std::size_t length);
  //开始接收数据
  void StartRecvDate();
  //开始tls 或者 websocket 握手
  void DoOtherHandshake(int32_t code, const std::string& msg);

protected:
  std::unique_ptr<NetConnection::NetConnectionRequest> request_;

private:
  std::mutex mutex_;
  NetConnection::NetConnectionDelegate* delegate_ = nullptr;
  std::shared_ptr<BASE_LOOPER::MessagePump> pump_;
  //用来保存读取到的数据
  boost::shared_array<char> read_buffer_;
  //一个封装类，给asio 读取数据用
  boost::asio::mutable_buffer asio_buffers_;

  //需要发送的数据
  std::deque<std::shared_ptr<std::string>> send_buffers;
  //用来DNS 解析
  std::shared_ptr<DnsResolver> dns_resolver_;
  //是否结束标志
  std::atomic_bool stoped_;
  //是否已经清理过
  bool already_clean_up = false;
  std::shared_ptr<Socks5Client> socks5_client_;
  std::string connection_id_;
  static std::atomic<int32_t> id_;
  DECLARE_OBJECT_RECORD(NetConnectionImpl)
};

END_NAMESPACE_NET
#endif
