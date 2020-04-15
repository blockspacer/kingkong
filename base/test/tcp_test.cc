#include "net/net_connection.h"
#include "log.h"
#include "net/dns_resolver.h"
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <boost/asio/ssl.hpp> 
#include <boost/filesystem.hpp>
#include "message_loop/message_loop.h"
#include <boost/test/unit_test.hpp>


enum { max_length = 1024 };

class client
{
public:
  client(boost::asio::io_service& io_service,
    boost::asio::ssl::context& context,
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    : socket_(io_service, context)
  {
    socket_.set_verify_mode(boost::asio::ssl::verify_peer);
    socket_.set_verify_callback(
      boost::bind(&client::verify_certificate, this, _1, _2));

    boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
      boost::bind(&client::handle_connect, this,
        boost::asio::placeholders::error));
  }

  bool verify_certificate(bool preverified,
    boost::asio::ssl::verify_context& ctx)
  {
    // The verify callback can be used to check whether the certificate that is  
    // being presented is valid for the peer. For example, RFC 2818 describes  
    // the steps involved in doing this for HTTPS. Consult the OpenSSL  
    // documentation for more details. Note that the callback is called once  
    // for each certificate in the certificate chain, starting from the root  
    // certificate authority.  

    // In this example we will simply print the certificate's subject name.  
    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
    std::cout << "Verifying " << subject_name << "\n";

    return preverified;
  }

  void handle_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_handshake(boost::asio::ssl::stream_base::client,
        boost::bind(&client::handle_handshake, this,
          boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Connect failed: " << error.message() << "\n";
    }
  }

  void handle_handshake(const boost::system::error_code& error)
  {
    if (!error)
    {
      std::cout << "Enter message: ";
      std::cin.getline(request_, max_length);
      size_t request_length = strlen(request_);

      boost::asio::async_write(socket_,
        boost::asio::buffer(request_, request_length),
        boost::bind(&client::handle_write, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      std::cout << "Handshake failed: " << error.message() << "\n";
    }
  }

  void handle_write(const boost::system::error_code& error,
    size_t bytes_transferred)
  {
    if (!error)
    {
      socket_.async_read_some(boost::asio::buffer(reply_, bytes_transferred),
        boost::bind(&client::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      std::cout << "Write failed: " << error.message() << "\n";
    }
  }

  void handle_read(const boost::system::error_code& error,
    size_t bytes_transferred)
  {
    if (!error)
    {
      std::cout << "Reply: ";
      std::cout.write(reply_, bytes_transferred);
      std::cout << "\n";
    }
    else
    {
      std::cout << "Read failed: " << error.message() << "\n";
    }
  }

private:
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
  char request_[max_length];
  char reply_[max_length];
};

class DnsResolverDelegateImpl : public BASE_NET::DnsResolver::DnsResolverDelegate {
public:
  void OnDnsResolvered(const boost::asio::ip::tcp::resolver::results_type& result, int code, const std::string& msg) override {
    for (auto& item : result) {
       LogInfo << "item:" << item.endpoint();
    }
  }
};

BOOST_AUTO_TEST_CASE(TCPTLS) {
  boost::asio::io_service io_service;

  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query("www.baidu.com", "443");
  boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

  boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
  ctx.load_verify_file("D:\\opensource\\kingkong\\third_party\\boost\\libs\\asio\\example\\cpp11\\ssl\\ca.pem");
  client c(io_service, ctx, iterator);

  io_service.run();

}
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


class TcpDelegate :public BASE_NET::NetConnection::NetConnectionDelegate {
  void OnConnect(BASE_NET::NetConnection* tcp, int code, const std::string& msg) override {
    LogInfo << "code: " << code << " msg:" << msg;
    if (0 == code) {
      std::stringstream data;
      data << "GET " << "/" << " HTTP/1.1\r\n";
      data << "Host: " << "www.baidu.com" << "\r\n";
      data << "Accept: */*\r\n";
      data << "Connection: close\r\n\r\n";
      std::string buffer = data.str();
      //tcp->Send(buffer.c_str(), buffer.size());
    }
  }
  void OnRecvData(BASE_NET::NetConnection* tcp, const void* buffer, int32_t buffer_len) override {
    LogInfo << "buffer_len: " << buffer_len;
  }
  void OnDisconnect(BASE_NET::NetConnection* tcp, int code, const std::string& msg) override {
    LogInfo << "code: " << code << " msg:" << msg;
  }

  void OnHandshake(BASE_NET::NetConnection* tcp, int code, const std::string& msg) override {
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
};

BOOST_AUTO_TEST_CASE(TCP) {
  boost::uniform_int<> real2(200, 2000);
  boost::random::mt19937 gen2;
  BASE_LOOPER::MessageLoop::InitMessageLoop();
  auto io_pump = BASE_LOOPER::MessageLoop::IOMessagePump();
  for (size_t i = 0; i < 1000; i++) {
    auto tcp_request = std::make_unique<BASE_NET::NetConnection::NetConnectionRequest>();
    tcp_request->host = "echo.websocket.org";
    tcp_request->port = 80;
    tcp_request->net_type = BASE_NET::NetConnection::kNetTypeWebsocket;
    auto tcp =
        BASE_NET::CreateTcp(std::move(tcp_request), new TcpDelegate, io_pump);
    tcp->Connect();
    boost::this_thread::sleep(boost::posix_time::milliseconds(real2(gen2)));
   // boost::this_thread::sleep(boost::posix_time::seconds(10000));
    tcp->DisConnect();
  }
 
  boost::this_thread::sleep(boost::posix_time::seconds(1000));
}
