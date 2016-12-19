//
// Created by mark on 11/19/16.
//

#ifndef BLACKPANTHER_BASE_THREAD_H
#define BLACKPANTHER_BASE_THREAD_H

#include <blackpanther/base/Atomic.h>

#include <boost/noncopyable.hpp>

#include <memory>
#include <functional>

#include <assert.h>

namespace blackpanther{
    class Thread : boost::noncopyable{
    public:
        typedef std::function<void ()> ThreadFunc;

        explicit Thread(const ThreadFunc &, const std::string &name = std::string());
        explicit Thread(ThreadFunc &&, const std::string &name = std::string());

        ~Thread();

        void start();
        int join();

        bool started() const { return started_; }
        pid_t tid(){
            return *tid_;
        }

        const std::string &name() const { return name_; }

        static int numCreated(){ return numCreated_.get(); }

    private:
        void setDefaultName();

        bool started_;
        bool joined_;

        pthread_t pthreadId_;
        std::shared_ptr<pid_t> tid_;
        ThreadFunc func_;
        std::string name_;

        static AtomicInt32 numCreated_;
    };
}
#endif //BLACKPANTHER_BASE_THREAD_H
