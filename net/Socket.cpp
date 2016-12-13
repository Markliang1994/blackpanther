//
// Created by mark on 16-12-13.
//
#include <blackpanther/net/Socket.h>

#include <blackpanther/base/Logging.h>

#include <unistd.h>

#include <netinet/tcp.h>
#include <stdio.h>
#include <arpa/inet.h>

using namespace blackpanther;
using namespace blackpanther::net;

Socket::~Socket(){
        // TODO
    close(socketfd_);
}

bool Socket::getTcpInfo(struct tcp_info *) const {
    // TODO
    return false;
}

bool Socket::getTcpInfoString(char *buf, int len) {
    // TODO
    return false;
}