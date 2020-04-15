#ifndef _BASE_TCP_CONNECTION_BASE_H_
#define _BASE_TCP_CONNECTION_BASE_H_

#include "net/net_connection_impl.h"
#include <base_header.h>
#include <boost/asio.hpp>

BEGIN_NAMESPACE_NET

template<class StreamType>
class TcpConnectionBase : public NetConnectionImpl {
public:
  TcpConnectionBase(
      std::unique_ptr<NetConnection::NetConnectionRequest> request,
      NetConnection::NetConnectionDelegate* delegate,
      std::shared_ptr<BASE_LOOPER::MessagePump> pump)
      : NetConnectionImpl(std::move(request), delegate, std::move(pump)) {}

protected:
  void DoRecvData(boost::asio::mutable_buffer& buffers) override {
   auto self = shared_from_this();
   //读的时候指定最大读取就可以了
   socket_->async_read_some(buffers, [self](boost::system::error_code ec,
                                           std::size_t length) mutable {
     self->NotifyRecvData(ec, length);
     self.reset();
   });
  }

  void DoSendData(boost::asio::const_buffer& buffers) override {
    auto self = shared_from_this();
    boost::asio::async_write(
        *socket_, buffers,
        [self](boost::system::error_code ec, std::size_t length) {
          self->NotifySendComplete(ec, length);
        });
  }

protected:
  std::unique_ptr<StreamType> socket_;
};

END_NAMESPACE_NET
#endif
