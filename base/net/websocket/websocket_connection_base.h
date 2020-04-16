﻿#ifndef _BASE_WEBSOCKET_CONNECTION_BASE_H_
#define _BASE_WEBSOCKET_CONNECTION_BASE_H_

#include "net/net_connection_impl.h"
#include <base_header.h>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>

BEGIN_NAMESPACE_NET

template <class StreamType>
class WebsocketConnectionBase : public NetConnectionImpl {
public:
  WebsocketConnectionBase(std::unique_ptr<NetConnection::NetConnectionRequest> request,
    NetConnection::NetConnectionDelegate* delegate,
    std::shared_ptr<BASE_LOOPER::MessagePump> pump) :
    NetConnectionImpl(std::move(request), delegate, std::move(pump)) {
  }


protected:
  void DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints) override {
    auto self = shared_from_this();
    boost::beast::get_lowest_layer(*ws_.get()).async_connect(endpoints,
      [self](boost::system::error_code& ec,
        boost::asio::ip::tcp::endpoint endpoint) {
          //连接失败通知上层
          self->NotifyConnectComplete(ec);
      });
  }

  void DoRecvData(boost::asio::mutable_buffer& buffers) override {
    auto self = shared_from_this();
    //读的时候指定最大读取就可以了
    ws_->async_read_some(buffers, [self](boost::system::error_code& ec, std::size_t length) mutable {
      self->NotifyRecvData(ec, length);
      self.reset();
      });
  }

  void DoSendData(boost::asio::const_buffer& buffers) override {
    auto self = shared_from_this();
    ws_->async_write(
      buffers,
      [self](boost::system::error_code& ec, std::size_t length) mutable {
        self->NotifySendComplete(ec, length);
        self.reset();
      });
  }
  void DoCleanUp() override {
    boost::system::error_code ec;
    ws_->close(boost::beast::websocket::close_code::normal, ec);
  }
  void DoWebsocketHandshake() override {
    auto self = shared_from_this();
    ws_->async_handshake(request_->host, "/", [self](boost::system::error_code& ec) {
      self->NotifyWebsocketHandshakeComplte(ec);
      });
  }

protected:
  std::unique_ptr<StreamType> ws_;
};

END_NAMESPACE_NET
#endif