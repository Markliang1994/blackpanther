//
// Created by mark on 16-12-8.
//

#include <blackpanther/net/Poller.h>

#include <blackpanther/net/Channel.h>

using namespace blackpanther;
using namespace blackpanther::net;

Poller::Poller(EventLoop *loop) :ownerLoop_(loop){

}

Poller::~Poller() {

}

bool Poller::hasChannel(Channel *channel) const {
    assertInLoopThread();
    ChannelMap::const_iterator it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}