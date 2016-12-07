//
// Created by mark on 16-12-6.
//

#include <blackpanther/net/EventLoop.h>

namespace blackpanther{
    namespace net{
        EventLoop::EventLoop():threadId_(CurrentThread::tid()){
            assertInLoopThread();
            printf("thread is:%d\n", threadId_);
        }

        void EventLoop::assertInLoopThread() {
            assert(threadId_ == CurrentThread::t_cachedTid);
        }

        void EventLoop::loop(){
            assertInLoopThread();
        }

        EventLoop::~EventLoop() {

        }
    }
}