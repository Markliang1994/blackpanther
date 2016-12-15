//
// Created by mark on 16-12-15.
//
#include <boost/implicit_cast.hpp>

#include <blackpanther/net/Buffer.h>
#include <blackpanther/net/SocketsOps.h>
#include <errno.h>

using namespace blackpanther;
using namespace blackpanther::net;

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;

ssize_t Buffer::readFd(int fd, int *savedErrno) {
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    const int iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
    const ssize_t n = sockets::readv(fd, vec, iovcnt);
    if(n < 0){
        *savedErrno = errno;
    }
    else if(boost::implicit_cast<size_t>(n) <= writable)
        writerIndex_ += n;
    else{
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }
    return n;
}