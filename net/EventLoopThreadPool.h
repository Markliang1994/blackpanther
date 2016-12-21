//
// Created by mark on 16-12-21.
//

#ifndef BLACKPANTHER_NET_EVENTLOOPTHREADPOOL_H
#define BLACKPANTHER_NET_EVENTLOOPTHREADPOOL_H

#include <blackpanther/base/Noncopyable.h>

#include <functional>
#include <memory>
#include <vector>

namespace blackpanther{
    namespace net{
        class EventLoop;
        class EventLoopThread;

        class EventLoopThreadPool : Noncopyable{
        public:
            typedef std::function<void(EventLoop*)> ThreadInitCallback;

            EventLoopThreadPool(EventLoop *baseLoop, const std::string &nameArg);
            ~EventLoopThreadPool();

            void setThreadNum(int numThreads){ numThreads_ = numThreads; }
            void start(const ThreadInitCallback &cb = ThreadInitCallback());

            EventLoop* getNextLoop();

            EventLoop* getLoopForHash(size_t hashCode);
            std::vector<EventLoop*> getAllLoops();

            bool started() const { return started_; }
            const std::string name() const { return name_; }
        private:

            EventLoop *baseLoop_;
            std::string name_;
            bool started_;
            int numThreads_;
            int next_;
            std::vector<std::unique_ptr<EventLoopThread>> thread_s;
            std::vector<EventLoop*> loops_;
        };
    }
}
#endif //BLACKPANTHER_EVENTLOOPTHREADPOOL_H
