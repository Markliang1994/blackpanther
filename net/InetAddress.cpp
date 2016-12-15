//
// Created by mark on 16-12-13.
//
#include <blackpanther/net/InetAddress.h>

#include <blackpanther/base/Logging.h>
#include <blackpanther/net/Endian.h>
#include <blackpanther/net/SocketsOps.h>

#include <netdb.h>
#include <netinet/in.h>

static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;

using namespace blackpanther;
using namespace blackpanther::net;

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in6), "InetAddress shoule be same size with sockaddr_in6");


InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6) {
    if(ipv6){
        bzero(&addr6_, sizeof(addr6_));
        addr6_.sin6_family = AF_INET6;
        in6_addr ip = loopbackOnly? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;
        addr6_.sin6_port = sockets::hostToNetwork16(port);
    }
    else{
        bzero(&addr_, sizeof(addr_));
        addr_.sin_family = AF_INET;
        in_addr_t ip = loopbackOnly? kInaddrLoopback : kInaddrAny;
        addr_.sin_addr.s_addr = sockets::hostToNetwork32(ip);
        addr_.sin_port = sockets::hostToNetwork16(port);
    }
}

InetAddress::InetAddress(std::string ip, uint16_t port, bool ipv6) {
    if(ipv6){
        bzero(&addr6_, sizeof(addr6_));
        sockets::fromIpPort(ip.c_str(), port, &addr6_);
    }
    else{
        bzero(&addr_, sizeof(addr_));
        sockets::fromIpPort(ip.c_str(), port, &addr_);
    }
}

std::string InetAddress::toIpPort() const {
    char buf[64] = "";
    sockets::toIpPort(buf, sizeof(buf), getSockAddr());
    return buf;
}

uint32_t InetAddress::ipNetEndian() const {
    assert(family() == AF_INET);
    return addr_.sin_addr.s_addr;
}

std::string InetAddress::toIp() const {
    char buf[64] = "";
    sockets::toIp(buf, sizeof(buf), getSockAddr());
    return buf;
}

uint16_t InetAddress::toPort() const {
    return sockets::networkToHost16(portNetEndian());
}

static __thread char t_resolveBuffer[64*1024];

bool InetAddress::resolve(std::string hostname, InetAddress *out){
    assert(out != nullptr);
    struct hostent hent;
    struct hostent *he = nullptr;
    int herrno = 0;
    bzero(&hent, sizeof(hent));

    int ret = gethostbyname_r(hostname.c_str(), &hent, t_resolveBuffer, sizeof(t_resolveBuffer), &he, &herrno);
    if(ret == 0 && he != nullptr){
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
        out->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    }
    else
        if(ret){
            LOG_SYSERR << "InetAddress::resolve";
        }
    return false;
}