//
// Created by mark on 16-12-20.
//

#ifndef BLACKPANTHER_NET_TCPCONNECTION_H
#define BLACKPANTHER_NET_TCPCONNECTION_H

#include <blackpanther/net/Callbacks.h>
#include <blackpanther/net/Buffer.h>
#include <blackpanther/net/InetAddress.h>

#include <memory>
#include <blackpanther/base/noncopyable.h>

#include <boost/any.hpp>
#include <boost/scoped_ptr.hpp>

struct tcp_info;
namespace blackpanther{
    namespace net{
        class Channel;
        class EventLoop;
        class Socket;

        class TcpConnection : blackpanther::noncopyable, std::enable_shared_from_this<TcpConnection>{
        public:
            TcpConnection(EventLoop *loop, const std::string name,
                          int sockfd,
                          const InetAddress &localaddr,
                          const InetAddress &peeraddr);
            ~TcpConnection();

            EventLoop* getLoop() const { return loop_; }
            const std::string& name() const { return name_; }
            const InetAddress& localAddress() const { return localaddr_; }
            const InetAddress& peerAddress() const { return peeraddr_; }
            bool connected() const { return state_ == kConnected; }
            bool disconnected() const { return state_ == kDisconnected; }

            bool getTcpInfo(struct tcp_info*) const;
            std::string getTcpInfoString() const;

            void send(const void *message, int len);
            void send(const std::string &&message);
            void send(Buffer &&message);
            void send(Buffer *message);
            void shutdown(); // Not thread safe

            void forceClose();
            void forceCloseWithDelay();
            void setTcpNoDelay();
            void startRead();
            void stopRead();
            bool isReading() const { return reading_; }

            void setConnectionCallback(const ConnectionCallback &cb){

            }
            void setMessageCallback(const MessageCallback &cb){

            }
            void setWriteCompleteCallback(const WriteCompleteCallback &cb){

            }
            void setHighWaterMarkCallback(const HighWaterMarkCallback &cb, size_t highWaterMark){

            }

            Buffer* inputBuffer(){ return &inputBuffe_; }
            Buffer* outputBuffer(){ return &outputBuffe_; }

            void setCloseCallback(const CloseCallback &cb){

            }

            void connectEstablished();
            void connectDestroyed();

        private:
            enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
            void handleRead(Timestamp receiveTime);
            void handleWrite();
            void handleClose();
            void handleError();

            void sendInLoop(const std::string &message);
            void sendInLoop(const void *message, size_t len);
            void shutdownInLoop();

            void forceCloseInLoop();
            void setState(StateE s){ state_ = s; }
            const char* stateToString() const;
            void startReadInLoop();
            void stopReadInLoop();

            EventLoop *loop_;
            const std::string name_;
            StateE state_;


            const InetAddress localaddr_;
            const InetAddress peeraddr_;
            ConnectionCallback connectionCallback_;
            MessageCallback messageCallback_;
            WriteCompleteCallback writeCompleteCallback_;
            HighWaterMarkCallback highWaterMarkCallback_;
            CloseCallback closeCallback_;
            size_t highWaterMark_;
            Buffer inputBuffer_;
            Buffer outputBuffer_;
            boost::any context_;
            bool reading_;
        };
    }
}
#endif //BLACKPANTHER_TCPCONNECTION_H
