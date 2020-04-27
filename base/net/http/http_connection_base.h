#ifndef _HTTP_BASE_CLIENT_H_
#define _HTTP_BASE_CLIENT_H_
#include "http_client.h"
#include "net/net_connection_impl.h"
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>

BEGIN_NAMESPACE_NET

template <class StreamType>
class HttpConnectionBase : public HttpClient,
                           public NetConnectionImpl,
                           public NetConnection::NetConnectionDelegate {
 public:
  HttpConnectionBase(std::unique_ptr<HttpClientRequest> http_request,
                     std::unique_ptr<NetConnection::NetConnectionRequest>
                         net_connection_request,
                     HttpClient::HttpClientDelegate* delegate,
                     std::shared_ptr<BASE_LOOPER::MessagePump> pump)
      : NetConnectionImpl(std::move(net_connection_request),
                          this,
                          std::move(pump)),
        http_request_(std::move(http_request)),
        delegate_(delegate) {}

  void Request() override { Connect(); }
  void Cancel() override {
    {
      std::lock_guard<std::mutex> lock(delegate_mutex_);
      delegate_ = nullptr;
    }
    DisConnect();
  }

  void DoRecvData(boost::asio::mutable_buffer buffers) override {
    auto self = shared_from_this();
    //读的时候指定最大读取就可以了
    boost::beast::http::async_read(
        *stream_, buffer_, res_,
        [this, self](boost::system::error_code ec, std::size_t length) mutable {
          //记录下数据
          //如果数据量很大的话，是会分多次接收？
          HandleHttpResponse();
          self->NotifyRecvData(ec, length);
          self.reset();
        });
  }

  void DoSendData(boost::asio::const_buffer buffers) override {
    auto self = shared_from_this();
    //组包
    req_.version(11);  // 1.1
    switch (http_request_->method) {
      case kHttpMethodGet: {
        req_.method(boost::beast::http::verb::get);
      } break;
      case kHttpMethodPost: {
        req_.method(boost::beast::http::verb::post);
      } break;
      case kHttpMethodPut: {
        req_.method(boost::beast::http::verb::put);
      } break;
      default:
        break;
    }
    req_.target(request_->path);
    req_.set(boost::beast::http::field::host, request_->host);

    boost::beast::http::async_write(
        *stream_, req_,
        [self](boost::system::error_code ec, std::size_t length) mutable {
          self->NotifySendComplete(std::move(ec), length);
          self.reset();
        });
  }

 protected:
  void OnConnect(NetConnection* tcp,
                 int code,
                 const std::string& msg) override {
    //连接成功发送数据,仅仅是为了触发基类的 dosendata
    if (0 == code) {
      Send("1", 1);
    } else {
      //通知连接失败
      NotifyResponse(boost::beast::http::status::not_found, msg);
    }
  }
  void OnRecvData(NetConnection* tcp,
                  const void* buffer,
                  int32_t buffer_len) override {}
  void OnDisconnect(NetConnection* tcp,
                    int code,
                    const std::string& msg) override {}

 private:
  void HandleHttpResponse() {
    auto& header = res_.base();
    for (auto& item : header) {
      // LogInfo << "key:" << item.name() << " value:" << item.value();
    }
    NotifyResponse(header.result(), res_.body());
  }
  void NotifyResponse(boost::beast::http::status http_code,
                      const std::string& message) {
    std::lock_guard<std::mutex> lock(delegate_mutex_);
    if (nullptr != delegate_) {
      delegate_->OnHttpResponse((int32_t)http_code, message);
    }
  }

 protected:
  std::unique_ptr<StreamType> stream_;
  std::unique_ptr<HttpClientRequest> http_request_;

 private:
  boost::beast::http::request<boost::beast::http::empty_body> req_;
  boost::beast::http::response<boost::beast::http::string_body> res_;

  boost::beast::flat_buffer buffer_;
  std::mutex delegate_mutex_;
  HttpClient::HttpClientDelegate* delegate_ = nullptr;
};

END_NAMESPACE_NET
#endif