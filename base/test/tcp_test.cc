#include "net/net_connection.h"
#include "log.h"
#include <base/crypto_util.h>
#include "net/dns_resolver.h"
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <boost/asio/ssl.hpp> 
#include <boost/filesystem.hpp>
#include "message_loop/message_loop.h"
#include <net/http/http_client.h>
#include <boost/test/unit_test.hpp>
#include "net/http/http_client.h"
#include "net/http/http_downloader.h"

#ifdef ENABLE_DNS_TEST

class DnsResolverDelegateImpl
  : public BASE_NET::DnsResolver::DnsResolverDelegate {
public:
  void OnDnsResolvered(
      const boost::asio::ip::tcp::resolver::results_type& result,
      int code,
      const std::string& msg) override {
    for (auto& item : result) {
      LogInfo << "item:" << item.endpoint();
    }
  }
};

DnsResolverDelegateImpl* dns_callback = new DnsResolverDelegateImpl;

BOOST_AUTO_TEST_CASE(DnsResolver) {
  BASE_UTIL::AtExitManager at;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  boost::thread_group group;

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(200, 2000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (int i = 0; i < 50; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::DnsResolver::DnsResolverRequest>();
        tcp_request->host = "www.baidu.com";
        auto tcp = BASE_NET::CreateDnsResolver(std::move(tcp_request),
                                               dns_callback, io_pump);
        tcp->Resolver();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        tcp->Cancel();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(200, 2000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (int i = 0; i < 50; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::DnsResolver::DnsResolverRequest>();
        tcp_request->host = "www.163.com";
        auto tcp = BASE_NET::CreateDnsResolver(std::move(tcp_request),
                                               dns_callback, io_pump);
        tcp->Resolver();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        tcp->Cancel();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(200, 2000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::WorkMessagePump();
      for (int i = 0; i < 50; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::DnsResolver::DnsResolverRequest>();
        tcp_request->host = "www.qq.com";
        auto tcp = BASE_NET::CreateDnsResolver(std::move(tcp_request),
                                               dns_callback, io_pump);
        tcp->Resolver();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        tcp->Cancel();
      }
    });
  }

  group.join_all();
  BASE_LOOPER::MessageLoop::UnintMessageLoop();
}
#endif // ENABLE_DNS_TEST

class TcpDelegate :public BASE_NET::NetConnection::NetConnectionDelegate {
  void OnConnect(BASE_NET::NetConnection* tcp, int code, const std::string& msg) override {
    LogInfo << "code: " << code << " msg:" << msg << " net_type:" << tcp->net_type();
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
  void OnRecvData(BASE_NET::NetConnection* tcp, const void* buffer, int32_t buffer_len) override {
    LogInfo << "buffer_len: " << buffer_len;
  }
  void OnDisconnect(BASE_NET::NetConnection* tcp, int code, const std::string& msg) override {
    LogInfo << "code: " << code << " msg:" << msg << " net_type:" << tcp->net_type();
  }
};
TcpDelegate* callback = new TcpDelegate;


//#define ENABLE_TCP_TEST

#ifdef ENABLE_TCP_TEST

BOOST_AUTO_TEST_CASE(TCP) {
  BASE_UTIL::AtExitManager at;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  boost::thread_group group;
  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(10, 50);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "baidu.com";
        tcp_request->port = 80;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcp;
        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(500, 1000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "baidu.com";
        tcp_request->port = 80;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcp;
        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(50, 300);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "baidu.com";
        tcp_request->port = 80;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcp;
        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 3000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::WorkMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "baidu.com";
        tcp_request->port = 80;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcp;
        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  group.join_all();
  BASE_LOOPER::MessageLoop::UnintMessageLoop();
}

#endif // ENABLE_TCP_TEST


//#define ENABLE_TCP_TLS_TEST
#ifdef ENABLE_TCP_TLS_TEST

BOOST_AUTO_TEST_CASE(TCPTLS) {
  BASE_UTIL::AtExitManager at;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  boost::thread_group group;
  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(10, 50);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "www.baidu.com";
        tcp_request->port = 443;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcpTls;
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(500, 1000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "www.baidu.com";
        tcp_request->port = 443;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcpTls;
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(50, 300);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "www.baidu.com";
        tcp_request->port = 443;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcpTls;
        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 3000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "www.baidu.com";
        tcp_request->port = 443;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcpTls;
        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  group.join_all();
  BASE_LOOPER::MessageLoop::UnintMessageLoop();
}

#endif // ENABLE_TCP_TLS_TEST


//#define ENABLE_WEBSOCKET_TEST

#ifdef ENABLE_WEBSOCKET_TEST

BOOST_AUTO_TEST_CASE(WEBSOCKETTEST) {
  BASE_UTIL::AtExitManager at;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  boost::thread_group group;
  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 5000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "121.40.165.18";
        tcp_request->port = 8800;

        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeWebsocket;
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(500, 1000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "121.40.165.18";
        tcp_request->port = 8800;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeWebsocket;
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(50, 300);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "echo.websocket.org";
        tcp_request->port = 8800;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeWebsocket;

        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 3000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "121.40.165.18";
        tcp_request->port = 8800;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeWebsocket;
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  group.join_all();
  BASE_LOOPER::MessageLoop::UnintMessageLoop();
}
#endif // ENABLE_WEBSOCKET_TEST

//#define  ENABLE_WEBSOCKET_TLS_TEST
#ifdef ENABLE_WEBSOCKET_TLS_TEST

BOOST_AUTO_TEST_CASE(WEBSOCKETTLSTEST) {
  BASE_UTIL::AtExitManager at;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  boost::thread_group group;
  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 5000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "echo.websocket.org";
        tcp_request->port = 443;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeWebsocketTls;

        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(500, 1000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "echo.websocket.org";
        tcp_request->port = 443;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeWebsocketTls;
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        //    boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(50, 300);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "echo.websocket.org";
        tcp_request->port = 443;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeWebsocketTls;
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        //boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 3000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "echo.websocket.org";
        tcp_request->port = 443;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeWebsocketTls;

        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateTcp(std::move(tcp_request), callback, io_pump);
        tcp->Connect();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        // boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->DisConnect();
      }
    });
  }

  group.join_all();
  BASE_LOOPER::MessageLoop::UnintMessageLoop();
}


#endif  // ENABLE_TCP_TEST

class HttpDelegate :public BASE_NET::HttpClient::HttpClientDelegate {
public:
  void OnHttpHeads(const std::unordered_map<std::string, std::string>& heads) override {

  }
  void OnHttpResponse( boost::string_view content) override {   
     // LogInfo << " content:" << content;
  }

  void OnHttpComplete(boost::beast::http::status http_code) override {
    LogInfo << "http_code: " << http_code;
  }
};


HttpDelegate* g_httpdelete = new HttpDelegate;
//#define  HTTPTEST
#ifdef HTTPTEST

BOOST_AUTO_TEST_CASE(HttpTest) {

  BASE_UTIL::AtExitManager at;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  boost::thread_group group;
  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 5000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 1000; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::HttpClient::HttpClientRequest>();
        tcp_request->url = "http://www.google.com";
        tcp_request->method = BASE_NET::HttpClient::kHttpMethodGet;
        auto tcp =
          BASE_NET::CreateHttpClient(std::move(tcp_request), g_httpdelete, io_pump);
        tcp->Request();
       // boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        boost::this_thread::sleep(boost::posix_time::seconds(1000000));
        tcp->Cancel();
      }
      });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 5000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 10000; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::HttpClient::HttpClientRequest>();
        tcp_request->url = "https://www.google.com";
        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateHttpClient(std::move(tcp_request), g_httpdelete, io_pump);
        tcp->Request();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        //boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->Cancel();
      }
      });
  }

  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 5000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 10000; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::HttpClient::HttpClientRequest>();
        tcp_request->url = "https://www.baidu.com";
        tcp_request->proxy_host = "222.186.37.106";
        tcp_request->proxy_port = 10249;
        tcp_request->proxy_username = "tw2800";
        tcp_request->proxy_passwd = "tw2800";
        auto tcp =
          BASE_NET::CreateHttpClient(std::move(tcp_request), g_httpdelete, io_pump);
        tcp->Request();
        boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        //boost::this_thread::sleep(boost::posix_time::seconds(10000));
        tcp->Cancel();
      }
      });
  }
  group.join_all();
  BASE_LOOPER::MessageLoop::UnintMessageLoop();
}

#endif  // HTTPTEST

class HttpTestDelegate : public BASE_NET::HttpDownloader::HttpDownloaderDelegate {
protected:
  void OnStartDownload() override {
    LogInfo << "";
  }
 void OnDownloadProgress(int64_t current_size, int64_t totla_size) override {
   //LogInfo << "current_size:" << current_size << "  totla_size:" << totla_size;
 }
  void OnDownloadComplte(int32_t status) override {
    LogInfo << "status:" << status;
  }
};

HttpTestDelegate* g_httpdownload = new HttpTestDelegate;

#define HTTP_DOWNLOAD_TEST
#ifdef HTTP_DOWNLOAD_TEST
BOOST_AUTO_TEST_CASE(HttpDownload) {
  BASE_UTIL::AtExitManager at;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  boost::thread_group group;

  for (int i = 0; i < 1; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 5000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 1; i++) {
        auto tcp_request =
            std::make_unique<BASE_NET::HttpDownloader::HttpDownloaderRequest>();
        tcp_request->url = "https://res06.bignox.com/full/20200113/d07258f92e3e4c488ef5b482316f5d11.exe?filename=nox_setup_v6.6.0.0_full.exe";
        tcp_request->path = "D:\\test\\text.exe";
        auto tcp = BASE_NET::CreateHttpDownloader(std::move(tcp_request),
                                              g_httpdownload, io_pump);
        tcp->Start();
        // boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
        boost::this_thread::sleep(boost::posix_time::seconds(1000000));
        tcp->Cancel();
      }
    });
  }



  group.join_all();
  BASE_LOOPER::MessageLoop::UnintMessageLoop;
}
#endif // HTTP_DOWNLOAD_TEST
