#ifndef _HTTP_BASE_CLIENT_H_
#define _HTTP_BASE_CLIENT_H_
#include "http_client.h"
#include "net/net_connection_impl.h"
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <unordered_map>

BEGIN_NAMESPACE_NET

template <class StreamType>
class HttpConnectionBase : public HttpClient,
                           public NetConnectionImpl,
                           public NetConnection::NetConnectionDelegate,
                           public boost::beast::http::basic_parser<false> {
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
    boost::beast::http::async_read_some(
        *stream_, buffer_, *this,
        [this, self](boost::system::error_code ec, std::size_t length) mutable {
          //记录下数据
          //如果数据量很大的话，是会分多次接收？
          if (!recv_complete) {
            self->NotifyRecvData(ec, length);
          }
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
    req_.keep_alive(true);
    req_.set(boost::beast::http::field::host, request_->host);

    boost::beast::http::async_write(
        *stream_, req_,
        [self](boost::system::error_code ec, std::size_t length) mutable {
          self->NotifySendComplete(std::move(ec), length);
          self.reset();
        });
  }

 protected:
   boost::beast::tcp_stream& GetLowestLayer() override {
     return boost::beast::get_lowest_layer(*stream_);
  }
  void OnConnect(NetConnection* tcp,
                 int code,
                 const std::string& msg) override {
    //连接成功发送数据,仅仅是为了触发基类的 dosendata
    if (0 == code) {
      Send("1", 1);
    } else {
      //通知连接失败
      http_code_ = boost::beast::http::status::not_found;
      NotifyComplete();
    }
  }
  void OnRecvData(NetConnection* tcp,
                  const void* buffer,
                  int32_t buffer_len) override {}
  void OnDisconnect(NetConnection* tcp,
                    int code,
                    const std::string& msg) override {
    NotifyComplete();
  }


protected:
  void on_request_impl(boost::beast::http::verb method,
                       boost::string_view method_str,
                       boost::string_view target,
                       int version,
                       boost::system::error_code& ec) override {
    throw std::logic_error("The method or operation is not implemented.");
  }

  void on_response_impl(int code,
                        boost::string_view reason,
                        int version,
                        boost::system::error_code& ec) override {
    //接收到HTTP 状态的时候回调
    http_code_ = (boost::beast::http::status)code;
  }

  void on_field_impl(boost::beast::http::field name,
                     boost::string_view name_string,
                     boost::string_view value,
                     boost::system::error_code& ec) override {
    heads_[std::string(name_string)] = std::string(value);
    if (name_string == "Content-Length") {
      uint64_t content_length = boost::lexical_cast<uint64_t>(value);
      if (content_length != 0) {
        body_limit(content_length);
      }
    }
  }

  void on_header_impl(boost::system::error_code& ec) override {
    //http头解析完成之后，会回调一次，可以在这里通知上层解析完成
    std::lock_guard<std::mutex> lock(delegate_mutex_);
    if (nullptr != delegate_) {
      delegate_->OnHttpHeads(heads_);
    }
  }

  void on_body_init_impl(boost::optional<std::uint64_t> const& content_length,
                         boost::system::error_code& ec) override {
    //只会调用一次，开始接收数据的时候调用
  }

  std::size_t on_body_impl(boost::string_view body, boost::system::error_code& ec) override {
    //循环接收数据
    NotifyResponse(body);
    return body.size();
  }

  void on_chunk_header_impl(std::uint64_t size,
                                    boost::string_view extensions,
                                    boost::system::error_code& ec) override {
    //throw std::logic_error("The method or operation is not implemented.");
  }

  std::size_t on_chunk_body_impl(std::uint64_t remain,
                                 boost::string_view body,
                                 boost::system::error_code& ec) override {
    
    NotifyResponse(body);
    return body.size();
  }

  void on_finish_impl(boost::system::error_code& ec) override {
    //消息全部解析完成之后回调
    NotifyComplete();
    recv_complete = true;
  }


private:

  void NotifyComplete() {
   std::lock_guard<std::mutex> lock(delegate_mutex_);
   if (nullptr != delegate_) {
     delegate_->OnHttpComplete(http_code_);
   }
  }

  void NotifyResponse(boost::string_view message) {
    std::lock_guard<std::mutex> lock(delegate_mutex_);
    if (nullptr != delegate_) {
      delegate_->OnHttpResponse(std::move(message));
    }
  }

 protected:
  std::unique_ptr<StreamType> stream_;
  std::unique_ptr<HttpClientRequest> http_request_;

 private:
  boost::beast::http::request<boost::beast::http::empty_body> req_;
  bool recv_complete = false;
  boost::beast::flat_buffer buffer_;
  std::mutex delegate_mutex_;
  HttpClient::HttpClientDelegate* delegate_ = nullptr;
  std::unordered_map<std::string, std::string> heads_;
  boost::beast::http::status http_code_ = boost::beast::http::status::not_found;
};

END_NAMESPACE_NET
#endif