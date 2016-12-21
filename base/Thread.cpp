//
// Created by mark on 11/19/16.
//
#include <blackpanther/base/CurrentThread.h>
#include <blackpanther/base/Thread.h>
#include <blackpanther/base/Exception.h>

#include <unistd.h>

#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <linux/unistd.h>

#include <stdio.h>

#include <type_traits>

#include <boost/static_assert.hpp>

namespace blackpanther{
    namespace CurrentThread{
        __thread int t_cachedTid = 0;
        __thread char t_tidString[32];
        __thread int t_tidStringLength = 6;
        __thread const char *t_threadName = "unknown";
        const bool sameType = std::is_same<int, pid_t>::value;
        BOOST_STATIC_ASSERT(sameType);
    }

    namespace detail{
        pid_t gettid(){
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }

        void afterFork(){
            blackpanther::CurrentThread::t_cachedTid = 0;
            blackpanther::CurrentThread::t_threadName = "main";
            CurrentThread::tid();
        }

        class ThreadNameInitializer{
        public:
            ThreadNameInitializer(){
                blackpanther::CurrentThread::t_threadName = "main";
                CurrentThread::tid();
                pthread_atfork(nullptr, nullptr, &afterFork);
            }
        };

        ThreadNameInitializer init;

        struct ThreadData{
            typedef blackpanther::Thread::ThreadFunc ThreadFunc;
            ThreadFunc func_;
            std::string name_;
            std::weak_ptr<pid_t> wkTid_;

            ThreadData(const ThreadFunc &func, std::string &name, const std::shared_ptr<pid_t> &tid)
                : func_(func), name_(name), wkTid_(tid){
            }

            void runInThread(){
                pid_t tid = blackpanther::CurrentThread::tid();
                std::shared_ptr<pid_t> ptid = wkTid_.lock();
                if(ptid){
                    *ptid = tid;
                    ptid.reset();
                }

                blackpanther::CurrentThread::t_threadName = name_.empty() ? "blackpantherThread" : name_.c_str();

                ::prctl(PR_SET_NAME, blackpanther::CurrentThread::t_threadName);

                try{
                    func_();
                    blackpanther::CurrentThread::t_threadName = "finished";
                }
                catch(const Exception &ex){

                }
                catch(const std::exception &ex){

                }
                catch(...){
                    throw;
                }
            }

        };

        void *startThread(void *obj){
            ThreadData *data = static_cast<ThreadData*>(obj);
            data->runInThread();
            delete data;
            return nullptr;
        }
    }
}

using namespace blackpanther;

void CurrentThread::cacheTid() {
    if(t_cachedTid == 0){
        t_cachedTid = detail::gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d ", t_cachedTid);
    }
}

bool CurrentThread::isMainThread(){
    return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec){
    struct timespec ts = {0, 0};
    ts.tv_nsec = static_cast<time_t>(usec/1000); // Fix me
    ts.tv_nsec = static_cast<long>(usec % 1000);
    ::nanosleep(&ts, nullptr);
}

Thread::Thread(const ThreadFunc &func, const std::string &n):
    started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(new pid_t(0)),
    func_(func),
    name_(n){
    setDefaultName();
}

Thread::Thread(ThreadFunc &&func, const std::string &n)
    :started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(new pid_t(0)),
    func_(std::move(func)),
    name_(n){
    setDefaultName();
}

AtomicInt32 Thread::numCreated_;

void Thread::setDefaultName() {
    int num = numCreated_.incrementAndGet();
    if(name_.empty()){
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}

Thread::~Thread() {
    if(started_ && !joined_){
        pthread_detach(pthreadId_);
    }
}

void Thread::start() {
    assert(!started_);
    started_ = true;

    detail::ThreadData * data = new detail::ThreadData(func_, name_, tid_);
    if(pthread_create(&pthreadId_, nullptr, &detail::startThread, data)){
        started_ = false;
        delete data;
        printf("failed in pthread_create.\n");
    }
}

int Thread::join(){
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, nullptr);
}