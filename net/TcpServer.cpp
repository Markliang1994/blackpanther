//
// Created by mark on 16-12-21.
//
#include <blackpanther/net/TcpServer.h>
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/EventLoopThreadPool.h>
#include <blackpanther/net/Acceptor.h>
#include <blackpanther/net/SocketsOps.h>

#include <blackpanther/base/Logging.h>

using namespace blackpanther;
using namespace blackpanther::net;

TcpServer::TcpServer(EventLoop *loop, const InetAddress &listenaddr, const std::string &nameArg, Option option)
            : loop_(loop),
              ipPort_(listenaddr.toIpPort()),
              name_(nameArg),
              acceptor_(new Acceptor(loop, listenaddr, option == kReusePort)),
              threadPool_(new EventLoopThreadPool(loop, name_)),
              connectionCallback_(defaultConnectionCallback),
              messageCallback_(defaultMessageCallback),
              nextConnId_(1){
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer() {
    loop_->assertInLoopThread();
    LOG_TRACE << "TcpServer::~TcpServer [" << name_ <<"] destructing";

    for(ConnectionMap::iterator it(connections_.begin());
                it != connections_.end(); ++it){
        TcpConnectionPtr conn = it->second;
        it->second.reset();
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
        conn.reset();
    }
}


void TcpServer::setThreadNum(int numThreads){
    assert(0 <= numThreads);
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::start(){
    if(started_.getAndSet(1) == 0){
        threadPool_->start(threadInitCallback_);

        assert(!acceptor_->listenning());
        loop_->runInLoop(std::bind(&Acceptor::listen, getPointer(acceptor_)));
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress &peeraddr) {
    loop_->assertInLoopThread();
    EventLoop *ioLoop = threadPool_->getNextLoop();
    char buf[64];
    snprintf(buf, sizeof(buf), "-%s#%d", ipPort_.c_str(), nextConnId_);
    ++nextConnId_;
    std::string connname = name_ + buf;

    LOG_INFO << "TcpServer::newConnection [" << name_ << "] - new connection ["
                                                      << connname << "] from " << peeraddr.toIpPort();
    InetAddress localaddr(sockets::getLocalAddr(sockfd));

    TcpConnectionPtr conn(new TcpConnection(ioLoop,
                                            connname,
                                            sockfd,
                                            localaddr,
                                            peeraddr));

    connections_[connname] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));

    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn) {
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn) {
    loop_->assertInLoopThread();
    LOG_INFO << "TcpServer::removeConnectionInLoop [" << name_ <<"] - connection " << conn->name();
    size_t n = connections_.erase(conn->name());
    (void)n;
    assert(n == 1);
    EventLoop *ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}