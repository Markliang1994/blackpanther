//
// Created by mark on 11/24/16.
//

#ifndef BLACKPANTHER_BASE_THREADPOOL_H
#define BLACKPANTHER_BASE_THREADPOOL_H

#include <blackpanther/base/Condition.h>
#include <blackpanther/base/Mutex.h>
#include <blackpanther/base/Thread.h>
#include <blackpanther/base/Noncopyable.h>

#include <deque>
#include <vector>

namespace blackpanther{
    class ThreadPool : Noncopyable{
    public:
        typedef std::function<void()> Task;

        explicit ThreadPool(const std::string &nameArg = std::string("ThreadPool"));
        ~ThreadPool();

        // Must be called before start
        void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
        void setThreadInitCallback(const Task &cb){
            threadInitCallback_ = cb;
        }

        void start(int numThreads);
        void stop();

        const std::string& name() const {
            return name_;
        }
        size_t  queueSize() const;
        void run(const Task &f);

    private:
        bool isFull() const;
        void runInThread();
        Task take();

        mutable MutexLock mutex_;
        Condition notEmpty_;
        Condition notFull_;

        Task threadInitCallback_;
        std::string name_;
        std::vector<std::unique_ptr<blackpanther::Thread>> threads_;
        std::deque<Task> queue_;
        size_t maxQueueSize_;
        bool running_;
    };
}
#endif //BLACKPANTHER_THREADPOOL_H
