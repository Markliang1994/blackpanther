//
// Created by mark on 16-12-20.
//
#include <blackpanther/net/TcpClient.h>

#include <blackpanther/base/Logging.h>
#include <blackpanther/net/Connector.h>
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/SocketsOps.h>

using namespace blackpanther;
using namespace blackpanther::net;

namespace blackpanther{
    namespace net{
        namespace detail{
            void removeConnection(EventLoop *loop, const TcpConnectionPtr &conn){
                loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
            }

            void removeConnector(const ConnectorPtr &connector){

            }
        }
    }
}

TcpClient::TcpClient(EventLoop *loop, const InetAddress &serveraddr, const std::string &nameArg)
            : loop_(CHECK_NOTNULL(loop)),
              connector_(new Connector(loop, serveraddr)),
              name_(nameArg),
              connectionCallback_(defaultConnectionCallback),
              messageCallback_(defaultMessageCallback),
              retry_(false),
              connect_(true),
              nextConnId_(1){
            connector_->setNewConnectionCallback(std::bind(&TcpClient::newConnection, this, _1));
            LOG_INFO << "TcpClient::~TcpClient[" << name_ << "] - connector " << getPointer(connector_);
}

TcpClient::~TcpClient() {
    LOG_INFO << "TcpClient::~TcpClient[" << name_ << "] - connector " << getPointer(connector_);
    TcpConnectionPtr conn;
    bool unique = false;
    {
        MutexLockGuard lock(mutex_);
        unique = connection_.unique();
        conn = connection_;
    }
    if(conn){
        assert(loop_ == conn->getLoop());
        CloseCallback cb = std::bind(&detail::removeConnection, loop_, _1);
        loop_->runInLoop(std::bind(&TcpConnection::setCloseCallback, conn, cb));
        if(unique){
            conn->forceClose();
        }
    }
    else{
        connector_->stop();
        loop_->runAfter(1, std::bind(&detail::removeConnector, connector_));
    }
}

void TcpClient::connect() {
    LOG_INFO << "TcpClient::connect[" << name_ << "] - connecting to "
                                               << connector_->serverAddress().toIpPort();
    connect_ = true;
    connector_->start();
}

void TcpClient::disconnect() {
    connect_ = false;

    {
        MutexLockGuard lock(mutex_);
        if(connection_){
            connection_->shutdown();
        }
    }
}

void TcpClient::stop() {
    connect_ = false;
    connector_->stop();
}

void TcpClient::newConnection(int sockfd) {
    loop_->assertInLoopThread();
    InetAddress peeraddr(sockets::getPeerAddr(sockfd));
    char buf[32];
    snprintf(buf, sizeof(buf), ":%s#%d", peeraddr.toIpPort().c_str(), nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;

    InetAddress localaddr(sockets::getLocalAddr(sockfd));

    TcpConnectionPtr conn(new TcpConnection(loop_,
                                            connName,
                                            sockfd,
                                            localaddr,
                                            peeraddr));

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpClient::removeConnection, this, _1));
    {
        MutexLockGuard lock(mutex_);
        connection_ = conn;
    }
    conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr &conn) {
    loop_->assertInLoopThread();
    assert(loop_ = conn->getLoop());

   {
        MutexLockGuard lock(mutex_);
        assert(connection_ == conn);
        connection_.reset();
    }
    loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    if(retry() && connect_){
        LOG_INFO << "TcpClient::connect[" << name_ << "] - Reconnecting to "
                                                   << connector_->serverAddress().toIpPort();
        connector_->restart();
    }
}