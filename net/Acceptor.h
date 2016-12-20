//
// Created by mark on 16-12-20.
//

#ifndef BLACKPANTHER_NET_ACCEPTOR_H
#define BLACKPANTHER_NET_ACCEPTOR_H

#include <blackpanther/net/Channel.h>
#include <blackpanther/net/Socket.h>

#include <blackpanther/base/noncopyable.h>

#include <functional>

namespace blackpanther{
    namespace net{
        class EventLoop;
        class InetAddress;

        class Acceptor : public blackpanther::noncopyable{
        public:
            typedef std::function<void(int sockfd, const InetAddress&)> NewConnectionCallback;

            Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport);
            ~Acceptor();

            void setNewConnectionCallback(const NewConnectionCallback &cb){
                newConnectionCallback_ = cb;
            }

            bool listenning(){ return listenning_; }
            void listen();

        private:
            void handleRead();

            EventLoop *loop_;
            Socket acceptSocket_;
            Channel acceptChannel_;
            NewConnectionCallback newConnectionCallback_;
            bool listenning_;
            int idleFd_;
        };
    }
}
#endif //BLACKPANTHER_ACCEPTOR_H
