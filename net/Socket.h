//
// Created by mark on 16-12-13.
//

#ifndef BLACKPANTHER_NET_SOCKET_H
#define BLACKPANTHER_NET_SOCKET_H

#include <blackpanther/base/noncopyable.h>

struct tcp_info;

namespace blackpanther{
    namespace net{
        class InetAddress;

        class Socket : noncopyable{
        public:
            explicit Socket(int socketfd):
                    socketfd_(socketfd){ }

            ~Socket();

            bool getTcpInfo(struct tcp_info *) const;
            bool getTcpInfoString(char *buf, int len);

            void bindAddress(const InetAddress &localaddr);

            void listen();

            int accept(InetAddress *peeraddr);

            void shutdownWrite();

            void setTcpNoDelay(bool on);

            // Enable or Disable SO_REUSEADDR
            void setReuseAddr(bool on);

            // Enable or Disable SO_REUSEPORT
            void setReusePort(bool on);

            // Enable or Disable SO_KEEPALIVE
            void setKeepAlive(bool on);

        private:
            const int socketfd_;
        };
    }
}
#endif //BLACKPANTHER_SOCKET_H