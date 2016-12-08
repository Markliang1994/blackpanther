//
// Created by mark on 16-12-6.
//

#ifndef BLACKPANTHER_NET_EVENTLOOP_H
#define BLACKPANTHER_NET_EVENTLOOP_H

#include <functional>
#include <vector>

#include <boost/any.hpp>

#include <blackpanther/base/Mutex.h>
#include <blackpanther/base/CurrentThread.h>
#include <blackpanther/base/Timestamp.h>
#include <blackpanther/base/noncopyable.h>
#include <bits/unique_ptr.h>

namespace blackpanther{
    namespace net{
        class Channel;
        class Poller;
        class TimerQueue;

        class EventLoop : noncopyable{
        public:
            typedef std::function<void()> Functor;

            EventLoop();
            ~EventLoop();
            /*
             * Always loop.
             * Must be created in the thread who create the EventLoop Object
             */
            void loop();
            /*
             *  quit loop
             */
            void quit();

            // Interval
            void wakeup();
            void updateChannel(Channel *channel);
            void removeChannel(Channel *channel);
            bool hasChannel(Channel *channel);

            void assertInLoopThread(){
                if(!isInLoopThread())
                    abortNotInLoopThread();
            }
            bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
        private:
            void abortNotInLoopThread();

            typedef std::vector<Channel*> ChannelList;

            bool looping_;
            bool quit_;
            bool eventHandling_;
            bool callingPendingFunctors_;
            int64_t  iteration;
            const pid_t threadId_;
            Timestamp pollReturnTime_;
            std::unique_ptr<Poller> poller_;
            std::unique_ptr<TimerQueue> timerQueue_;
            int wakeupFd_;

            std::unique_ptr<Channel> wakeupChannel_;
            boost::any context_;

            ChannelList activeChannels_;
            Channel *currentActiveChannel_;
            mutable MutexLock mutex_;
            std::vector<Functor> pendingFunctors_;
        };
    }
}
#endif //BLACKPANTHER_EVENTLOOP_H
