#ifndef _BASE_TCP_IMPL_H_
#define _BASE_TCP_IMPL_H_

#include "dns_resolver.h"
#include "tcp.h"
#include <base_header.h>
#include <boost/shared_array.hpp>
#include <boost/asio.hpp>
#include <queue>

BEGIN_NAMESPACE_NET
//所有的网络操作都需要抛到pump 线程执行
class TcpImpl : public Tcp, 
  public DnsResolver::DnsResolverDelegate,
  public std::enable_shared_from_this<TcpImpl> {
public:
  TcpImpl(std::unique_ptr<Tcp::TcpRequest> request, Tcp::TcpDelegate* delegate, std::shared_ptr<BASE_LOOPER::MessagePump> pump);
  ~TcpImpl();


  void Connect() override;
  void DisConnect() override;

  void Send(const void* buffer, int32_t buffer_len) override;

private:
  void OnDnsResolvered(const boost::asio::ip::tcp::resolver::results_type& result, int code, const std::string& msg) override;

  //都是在同一个pump 线程执行
  void DoDnsResolve();
  void DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints);
  void DoRecvData();
  void DoSend(std::shared_ptr<std::string> send_buffer);
  void AsyncSend();
  void DoCleanUp();
  //回调连接状态
  void NotifyConnectStatus(int code, const std::string& msg);
  //断开连接的回调
  void NotifyDisconnect(boost::system::error_code& ec);
  //回调接收到了数据
  void NotifyRecvData(std::size_t length);
 
private:
  std::unique_ptr<Tcp::TcpRequest> request_;
  std::mutex mutex_;
  Tcp::TcpDelegate* delegate_ = nullptr;
  std::shared_ptr<BASE_LOOPER::MessagePump> pump_;
  boost::asio::ip::tcp::socket socket_;
  //用来保存读取到的数据
  boost::shared_array<char> read_buffer_;
  //需要发送的数据
  std::deque<std::shared_ptr<std::string>> send_buffers;
  //用来DNS 解析
  std::shared_ptr<DnsResolver> dns_resolver_;
  //是否结束标志
  std::atomic_bool stoped_ = false;
};

END_NAMESPACE_NET
#endif
