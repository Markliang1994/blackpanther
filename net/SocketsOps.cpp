//
// Created by mark on 16-12-15.
//
#include <blackpanther/net/SocketsOps.h>

#include <blackpanther/base/Logging.h>
#include <blackpanther/net/Endian.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace blackpanther;
using namespace blackpanther::net;

namespace {
    typedef struct sockaddr SA;
    void setNonBlockAndCloseOnExec(int sockfd){
        int flags = ::fcntl(sockfd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        int ret = ::fcntl(sockfd, F_SETFL, flags);

        flags = ::fcntl(sockfd, F_GETFD, 0);
        flags |= FD_CLOEXEC;
        ret = ::fcntl(sockfd, F_SETFD, flags);

        (void)ret;
    }
}

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in6 *addr) {
    return static_cast<const struct sockaddr*>(boost::implicit_cast<const void*>(addr));
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in6 *addr) {
    return static_cast<struct sockadd*>(boost::implicit_cast<void*>(addr));
}

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in *addr){
    return static_cast<const struct sockaddr*>(boost::implicit_cast<const void*>(addr));
}

const struct sockaddr_in* sockets::sockaddr_in_cast(const struct sockaddr *addr) {
    return static_cast<const struct sockaddr_in*>(boost::implicit_cast<const void*>(addr));
}

const struct sockaddr_in6* sockets::sockaddr_in6_cast(const struct sockaddr *addr){
    return static_cast<const struct sockaddr_in6*>(boost::implicit_cast<const void*>(addr));
}

