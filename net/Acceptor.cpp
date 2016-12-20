//
// Created by mark on 16-12-20.
//

#include <blackpanther/base/Logging.h>
#include <blackpanther/net/Acceptor.h>
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/InetAddress.h>
#include <blackpanther/net/SocketsOps.h>

#include <errno.h>
#include <fcntl.h>

using namespace blackpanther;
using namespace blackpanther::net;

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport)
        : loop_(loop),
          acceptSocket_(sockets::createNonblockingOrDie(listenAddr.family())),
          acceptChannel_(loop_, acceptSocket_.fd()),
          listenning_(false),
          idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)){
    assert(idleFd_ > 0);
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(reuseport);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
    ::close(idleFd_);
}

void Acceptor::listen() {
    loop_->assertInLoopThread();
    listenning_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}

void Acceptor::handleRead(){
    loop_->assertInLoopThread();
    InetAddress peeraddr;
    // FIXME loop until more
    int connfd = acceptSocket_.accept(&peeraddr);
    if(connfd >= 0){
        if(newConnectionCallback_){
            newConnectionCallback_(connfd, peeraddr);
        }
        else{
            sockets::close(connfd);
        }
    }
    else{
        LOG_SYSERR << "in Acceptor::handleRead";
        if(errno == EMFILE){
            ::close(idleFd_);
            idleFd_ = ::accept(acceptSocket_.fd(), nullptr, nullptr);
            ::close(idleFd_);
            idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}