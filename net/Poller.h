//
// Created by mark on 16-12-8.
//

#ifndef BLACKPANTHER_NET_POLLER_H
#define BLACKPANTHER_NET_POLLER_H

#include <map>
#include <vector>

#include <blackpanther/net/EventLoop.h>
#include <blackpanther/base/Timestamp.h>

namespace blackpanther{
    namespace net{
        class Channel;
        class Poller : Noncopyable{
        public:
            typedef std::vector<Channel*> ChannelList;

            Poller(EventLoop *loop);
            virtual ~Poller();

            virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
            virtual void updateChannel(Channel *channel) = 0;
            virtual void removeChannel(Channel * channel) = 0;
            virtual bool hasChannel(Channel *channel) const;

            static Poller* newDefaultPoller(EventLoop *loop);

            void assertInLoopThread() const {
                return ownerLoop_->assertInLoopThread();
            }

        protected:
            typedef std::map<int, Channel*> ChannelMap;
            ChannelMap channels_;
        private:
            EventLoop *ownerLoop_;
        };
    }
}
#endif //BLACKPANTHER_POLLER_H
