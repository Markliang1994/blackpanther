//
// Created by mark on 16-12-21.
//

#ifndef BLACKPANTHER_NET_EVENTLOOPTHREAD_H
#define BLACKPANTHER_NET_EVENTLOOPTHREAD_H

#include <blackpanther/base/Noncopyable.h>
#include <blackpanther/base/Condition.h>
#include <blackpanther/base/Mutex.h>
#include <blackpanther/base/Thread.h>

namespace blackpanther{
    namespace net{
        class EventLoop;

        class EventLoopThread : Noncopyable{
        public:
            typedef std::function<void(EventLoop*)> ThreadInitCallback;
            EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(),
                            const std::string &name = std::string());
            ~EventLoopThread();
            EventLoop* startLoop();

        private:
            void threadFunc();

            EventLoop *loop_;
            bool exiting_;
            Thread thread_;
            MutexLock mutex_;
            Condition cond_;
            ThreadInitCallback callback_;
        };
    }
}
#endif //BLACKPANTHER_EVENTLOOPTHREAD_H
