#include "net/tcp.h"
#include "log.h"
#include "net/dns_resolver.h"
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include "message_loop/message_loop.h"
#include <boost/test/unit_test.hpp>

class DnsResolverDelegateImpl : public BASE_NET::DnsResolver::DnsResolverDelegate {
public:
  void OnDnsResolvered(const boost::asio::ip::tcp::resolver::results_type& result, int code, const std::string& msg) override {
    for (auto& item : result) {
       LogInfo << "item:" << item.endpoint();
    }
  }
};

/*
BOOST_AUTO_TEST_CASE(DNS) {
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
  for (int i=0;i<1000;i++)  {
    auto tcp_request =
        std::make_unique<BASE_NET::DnsResolver::DnsResolverRequest>();
    tcp_request->host = "www.baidu.com";
    auto tcp = BASE_NET::CreateDnsResolver(std::move(tcp_request),
                                           new DnsResolverDelegateImpl,
                                           io_pump);
    tcp->Resolver();
   // boost::this_thread::sleep(boost::posix_time::milliseconds(20));
    tcp->Cancel();
  }
 
  boost::this_thread::sleep(boost::posix_time::seconds(1000));
}
*/


class TcpDelegate :public BASE_NET::Tcp::TcpDelegate {
  void OnConnect(BASE_NET::Tcp* tcp, int code, const std::string& msg) override {
    LogInfo << "code: " << code << " msg:" << msg;
    if (0 == code) {
      std::stringstream data;
      data << "GET " << "/" << " HTTP/1.1\r\n";
      data << "Host: " << "www.baidu.com" << "\r\n";
      data << "Accept: */*\r\n";
      data << "Connection: close\r\n\r\n";
      std::string buffer = data.str();
      tcp->Send(buffer.c_str(), buffer.size());
    }
  }
  void OnRecvData(BASE_NET::Tcp* tcp, const void* buffer, int32_t buffer_len) override {
    LogInfo << "buffer_len: " << buffer_len ;
  }
  void OnDisconnect(BASE_NET::Tcp* tcp, int code, const std::string& msg) override {
    LogInfo << "code: " << code << " msg:" << msg;
  }
};

BOOST_AUTO_TEST_CASE(TCP) {
  boost::uniform_int<> real2(20, 1000);
  boost::random::mt19937 gen2;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
  for (size_t i = 0; i < 10000; i++) {
    auto tcp_request = std::make_unique<BASE_NET::Tcp::TcpRequest>();
    tcp_request->host = "www.baidu.com";
    tcp_request->port = 8080;
    auto tcp =
        BASE_NET::CreateTcp(std::move(tcp_request), new TcpDelegate, io_pump);
    tcp->Connect();
    boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
    tcp->DisConnect();
  }
 
  boost::this_thread::sleep(boost::posix_time::seconds(1000));
}
