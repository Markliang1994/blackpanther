//
// Created by mark on 16-12-6.
//

#include <blackpanther/net/EventLoop.h>
#include <blackpanther/base/Logging.h>

namespace blackpanther{
    namespace net{
        EventLoop::EventLoop():threadId_(CurrentThread::tid()){
            assertInLoopThread();
            printf("thread is:%d\n", threadId_);
        }

        void EventLoop::loop(){
            if(!isInLoopThread())
                abortNotInLoopThread();
        }

        EventLoop::~EventLoop() {

        }
    }
}

using namespace blackpanther;
using namespace blackpanther::net;

void EventLoop::abortNotInLoopThread() {
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
                                                                << " was created in thread " << threadId_
                                                                                             << ", But current thread id is: " << CurrentThread::tid();
}