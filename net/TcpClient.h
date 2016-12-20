//
// Created by mark on 16-12-20.
//

#ifndef BLACKPANTHER_NET_TCPCLIENT_H
#define BLACKPANTHER_NET_TCPCLIENT_H

#include <blackpanther/base/Mutex.h>
#include <blackpanther/net/TcpConnection.h>

namespace blackpanther{
    namespace net{
        class Connector;
        typedef std::shared_ptr<Connector> ConnectorPtr;

        class TcpClient : public noncopyable{
        public:
            TcpClient(EventLoop *loop, const InetAddress &serveraddr, const std::string &nameArg);
            ~TcpClient();

            void connect();
            void disconnect();
            void stop();

            TcpConnectionPtr connection() const{
                MutexLockGuard lock(mutex_);
                return connection_;
            }

            EventLoop* getLoop() const { return loop_; }
            bool retry() const{ return retry_; }
            void enableRetry() { retry_ = true; }

            const std::string& name() const{
                return name_;
            }

            void setConnectionCallback(const ConnectionCallback &cb){
                connectionCallback_ = cb;
            }
            void setMessageCallback(const MessageCallback &cb){
                messageCallback_ = cb;
            }
            void setWriteCompleteCallback(const WriteCompleteCallback &cb){
                writeCompleteCallback_ = cb;
            }


        private:
            void newConnection(int sockfd);
            void removeConnection(const TcpConnectionPtr &conn);

            EventLoop *loop_;
            ConnectorPtr connector_;
            const std::string name_;
            ConnectionCallback connectionCallback_;
            MessageCallback messageCallback_;
            WriteCompleteCallback writeCompleteCallback_;
            bool retry_;
            bool connect_;
            int nextConnId_;
            mutable MutexLock mutex_;
            TcpConnectionPtr connection_;
        };
    }
}
#endif //BLACKPANTHER_TCPCLIENT_H
