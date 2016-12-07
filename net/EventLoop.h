//
// Created by mark on 16-12-6.
//

#ifndef BLACKPANTHER_NET_EVENTLOOP_H
#define BLACKPANTHER_NET_EVENTLOOP_H

#include <functional>
#include <vector>

#include <blackpanther/base/Mutex.h>
#include <blackpanther/base/CurrentThread.h>
#include <blackpanther/base/Timestamp.h>
#include <blackpanther/base/noncopyable.h>

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

            void loop();
            void assertInLoopThread();
        private:
            const pid_t threadId_;
        };
    }
}
#endif //BLACKPANTHER_EVENTLOOP_H
