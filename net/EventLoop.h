//
// Created by mark on 16-12-6.
//

#ifndef BLACKPANTHER_NET_EVENTLOOP_H
#define BLACKPANTHER_NET_EVENTLOOP_H

#include <functional>
#include <vector>
#include <memory>
#include <boost/any.hpp>

#include <blackpanther/base/Mutex.h>
#include <blackpanther/base/CurrentThread.h>
#include <blackpanther/base/Timestamp.h>
#include <blackpanther/base/Noncopyable.h>

#include <blackpanther/net/TimerId.h>
#include <blackpanther/net/Callbacks.h>

namespace blackpanther{
    namespace net{
        class Channel;
        class Poller;
        class TimerQueue;

        class EventLoop : Noncopyable{
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


            Timestamp pollReturnTime() const { return pollReturnTime_; }

            int64_t iteration() const { return iteration_; }


            void runInLoop(const Functor &cb);

            void queueInLoop(const Functor &cb);

            size_t queueSize() const;

            TimerId runAt(const Timestamp &time, const TimerCallback &cb);

            TimerId runAfter(double delay, const TimerCallback &cb);

            TimerId runEvery(double interval, const TimerCallback &&cb);
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

            void setContext(const boost::any &context){
                context_ = context;
            }

            static EventLoop* getEventLoopOfCurrentThread();
        private:
            void abortNotInLoopThread();
            void handleRead();
            void doPendingFunctors();

            void printActiveChannels() const ; // For Debug

            typedef std::vector<Channel*> ChannelList;

            bool looping_;
            bool quit_;
            bool eventHandling_;
            bool callingPendingFunctors_;
            int64_t  iteration_;
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
