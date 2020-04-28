#include "net/net_connection.h"
#include "log.h"
#include "net/dns_resolver.h"
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <boost/asio/ssl.hpp> 
#include <boost/filesystem.hpp>
#include "message_loop/message_loop.h"
#include <net/http/http_client.h>
#include <boost/test/unit_test.hpp>
#include "net/http/http_client.h"

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
        tcp_request->host = "echo.websocket.org";
        tcp_request->port = 80;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcp;
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
        tcp_request->port = 80;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcp;
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
        tcp_request->port = 80;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcp;
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
        tcp_request->host = "echo.websocket.org";
        tcp_request->port = 80;
        tcp_request->net_type = BASE_NET::NetConnection::kNetTypeTcp;
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
        tcp_request->host = "echo.websocket.org";
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
        tcp_request->host = "www.qq.com";
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
      boost::uniform_int<> real2(100, 3000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::FileMessagePump();
      for (size_t i = 0; i < 300; i++) {
        auto tcp_request =
            std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
        tcp_request->host = "www.163.com";
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
  void OnHttpResponse(int32_t http_code, const std::string& content) {
    LogInfo << "http_code: " << http_code << " content:" << content;
  }
};


HttpDelegate* g_httpdelete = new HttpDelegate;

BOOST_AUTO_TEST_CASE(HttpTest) {
  BASE_UTIL::AtExitManager at;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  boost::thread_group group;
  for (int i = 0; i < 5; i++) {
    group.create_thread([] {
      boost::uniform_int<> real2(100, 5000);
      boost::random::mt19937 gen2;
      auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
      for (size_t i = 0; i < 10000; i++) {
        auto tcp_request =
          std::make_unique<BASE_NET::HttpClient::HttpClientRequest>();
        tcp_request->url = "https://www.qq.com";
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
          tcp_request->url = "http://www.qq.com";
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
}