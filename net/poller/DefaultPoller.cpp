//
// Created by mark on 16-12-8.
//

#include <blackpanther/net/Poller.h>
#include <blackpanther/net/poller/EPollPoller.h>


using namespace blackpanther::net;

Poller* Poller::newDefaultPoller(EventLoop *loop) {
    /*
    if(::getenv("MUDUO_USE_POLL"))
        return new PollPoller(loop);
    else
     */
    return new EPollPoller(loop);
}