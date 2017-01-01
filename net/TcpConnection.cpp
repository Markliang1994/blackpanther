//
// Created by mark on 16-12-20.
//
#include <blackpanther/net/TcpConnection.h>
#include <blackpanther/base/Logging.h>
#include <blackpanther/base/WeakCallback.h>
#include <blackpanther/net/Channel.h>
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/SocketsOps.h>
#include <blackpanther/net/Socket.h>

#include <errno.h>

using namespace blackpanther;
using namespace blackpanther::net;

void blackpanther::net::defaultConnectionCallback(const TcpConnectionPtr &conn) {
    LOG_TRACE << conn->localAddress().toIpPort() << " -> "
                                                 << conn->peerAddress().toIpPort() << " is "
                                                 << (conn->connected() ? "UP" : "DOWN");
}

void blackpanther::net::defaultMessageCallback(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp receiveTime) {
    buffer->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop *loop, const std::string nameArg, int sockfd, const InetAddress &localaddr,
                             const InetAddress &peeraddr)
                            : loop_(CHECK_NOTNULL(loop)),
                              name_(nameArg),
                              state_(kConnecting),
                              socket_(new Socket(sockfd)),
                              channel_(new Channel(loop, sockfd)),
                              localaddr_(localaddr),
                              peeraddr_(peeraddr),
                              highWaterMark_(60*1024*1024),
                              reading_(true){
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, _1));
    channel_->setWriteCallback(
            std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    LOG_DEBUG << "TcpConnection::ctor[" << name_ << "] at " << this << " fd=" << sockfd;
}

TcpConnection::~TcpConnection() {
    LOG_DEBUG << "TcpConnection::dtor[" << name_ << "] at " << this
                                                            << " fd=" << channel_->fd()
                                                            << " state=" << stateToString();
    assert(state_ == kDisconnected);
}

bool TcpConnection::getTcpInfo(struct tcp_info *tcpi) const {
    return socket_->getTcpInfo(tcpi);
}

std::string TcpConnection::getTcpInfoString() const {
    char buf[1024];
    buf[0] = '\0';
    socket_->getTcpInfoString(buf, sizeof(buf));
    return buf;
}

void TcpConnection::send(const void *data, int len){
    send(std::string(static_cast<const char*>(data), len));
}

void TcpConnection::send(const std::string &message){
    if(state_ == kConnected){
        if(loop_->isInLoopThread()){
            sendInLoop(message);
        }
        else{
            void(TcpConnection::*fp)(const std::string &message) = &TcpConnection::sendInLoop;
            loop_->runInLoop(std::bind(fp,
                      this,
                      message.c_str()));
        }
    }
}

void TcpConnection::send(Buffer *buf){
    if(state_ == kConnected){
        if(loop_->isInLoopThread()){
            sendInLoop(buf->peek(), buf->readableBytes());
            buf->retrieveAll();
        }
        else{
            void (TcpConnection::*fp)(const std::string &message) = &TcpConnection::sendInLoop;
            loop_->runInLoop(std::bind(fp, this, buf->retrieveAllAsString()));
        }
    }
}

void TcpConnection::sendInLoop(const std::string &message){
    sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void *data, size_t len){
    loop_->assertInLoopThread();
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;
    if(state_ == kDisconnected){
        LOG_WARN << "disconnected, give up writing";
        return;
    }
    // if no thing in the output queue, write directly
    if(!channel_->isWritting() && outputBuffer_.readableBytes() == 0){
        nwrote = sockets::write(channel_->fd(), data, len);
        if(nwrote >= 0){
            remaining = len - nwrote;
            if(remaining == 0 && writeCompleteCallback_){
                loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
            }
        }
        else{
            nwrote = 0;
            if(errno != EWOULDBLOCK){
                LOG_SYSERR << "TcpConnection::sendInLoop";
                if(errno == EPIPE || errno == ECONNRESET){
                    faultError = true;
                }
            }
        }
    }

    assert(remaining <= len);
    if(!faultError && remaining > 0){
        size_t oldlen = outputBuffer_.readableBytes();
        if(oldlen + remaining >= highWaterMark_
                && oldlen < highWaterMark_
                && highWaterMarkCallback_){
            loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldlen + remaining));
        }
        outputBuffer_.append(static_cast<const char*>(data)+nwrote, remaining);
        if(!channel_->isWritting()){
            channel_->enableWritting();
        }
    }
}

void TcpConnection::shutdown() {
    //FIXME use compare and swap
    if(state_ == kConnected){
        setState(kDisconnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
    }
}

void TcpConnection::shutdownInLoop() {
    loop_->assertInLoopThread();
    if(!channel_->isWritting())
        socket_->shutdownWrite();
}

void TcpConnection::forceClose() {
    if(state_ == kConnected || state_ == kDisconnecting){
        setState(kDisconnecting);
        loop_->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
    }
}

void TcpConnection::forceCloseWithDelay(double seconds) {
    if(state_ == kConnected || state_ == kDisconnecting){
        setState(kDisconnecting);
        loop_->runAfter(seconds, makeWeakCallback(shared_from_this(), &TcpConnection::forceClose));
    }
}

void TcpConnection::forceCloseInLoop() {
    loop_->assertInLoopThread();
    if(state_ == kConnected || state_ == kDisconnecting)
        handleClose();
}

const char* TcpConnection::stateToString() const {
    switch(state_){
        case kDisconnected:
            return "kDisconnected";
        case kConnecting:
            return "kConnecting";
        case kConnected:
            return "kConnected";
        case kDisconnecting:
            return "kDisconnecting";
        default:
            return "unknown state";
    }
}

void TcpConnection::setTcpNoDelay(bool on) {
    socket_->setTcpNoDelay(on);
}

void TcpConnection::startRead() {
    loop_->runInLoop(std::bind(&TcpConnection::startReadInLoop, this));
}

void TcpConnection::startReadInLoop() {
    loop_->assertInLoopThread();
    if(!reading_ || !channel_->isReading()){
        channel_->enableReading();
        reading_ = true;
    }
}

void TcpConnection::stopReadInLoop() {
    loop_->assertInLoopThread();
    if(reading_ || channel_->isReading()){
        channel_->disableReading();
        reading_ = false;
    }
}

void TcpConnection::connectEstablished() {
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
    setState(kConnected);
    channel_->tie(shared_from_this());
    channel_->enableReading();

    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed() {
    loop_->assertInLoopThread();
    if(state_ == kConnected){
        setState(kDisconnected);
        channel_->disableAll();

        connectionCallback_(shared_from_this());
    }
    channel_->remove();
}

void TcpConnection::handleRead(Timestamp receiveTime) {
    loop_->assertInLoopThread();
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if(n > 0){
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }
    else if(n == 0){
        handleClose();
    }
    else{
        errno = savedErrno;
        LOG_SYSERR << "TcpConnection::handleRead";
        handleError();
    }
}

void TcpConnection::handleWrite() {
    loop_->assertInLoopThread();
    if(channel_->isWritting()){
        ssize_t n = sockets::write(channel_->fd(),
                                    outputBuffer_.peek(),
                                    outputBuffer_.readableBytes());

        if(n > 0){
            outputBuffer_.retrieve(n);
            if(outputBuffer_.readableBytes() == 0){
                channel_->disableWritting();
                if(writeCompleteCallback_){
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }
                if(state_ == kDisconnecting) {
                    shutdownInLoop();
                }
            }
        }
        else{
            LOG_SYSERR << "TcpConnection::handleWrite";
        }
    }
    else{
        LOG_TRACE << "Connection fd = " << channel_->fd()
                                        << " is down, no more writing";
    }
}

void TcpConnection::handleClose(){
    loop_->assertInLoopThread();
    LOG_TRACE << "fd = " << channel_->fd() << " state = " << stateToString();
    assert(state_ == kConnected || state_ == kDisconnecting);
    // we don't close fd, let dtor do it so that we can find leak easily
    setState(kDisconnected);
    channel_->disableAll();

    TcpConnectionPtr guardThis(shared_from_this());
    connectionCallback_(guardThis);
    // must be the last line
    closeCallback_(guardThis);
}

void TcpConnection::handleError() {
    int err = sockets::getSocketError(channel_->fd());
    LOG_ERROR << "TcpConnection::handleError [" << name_
                                                <<"] - SO_ERROR = " << err << " " << strerror_tl(err);
}