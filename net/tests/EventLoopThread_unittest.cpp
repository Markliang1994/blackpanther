//
// Created by mark on 16-12-21.
//
#include <blackpanther/net/EventLoopThread.h>
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/base/Thread.h>
#include <blackpanther/base/CountDownLatch.h>
#include <blackpanther/base/Logging.h>

#include <stdio.h>

using namespace blackpanther;
using namespace blackpanther::net;

void print(EventLoop *p = nullptr){
    printf("print: pid = %d, tid = %d loop = %p\n",
                getpid(), CurrentThread::tid(), p);
}

void quit(EventLoop *p){
    print(p);
    p->quit();
}

int main(void){
    {
        EventLoopThread elt1; // Do nothing just call ctor.
    }
    {
        EventLoopThread elt2;
        EventLoop *loop = elt2.startLoop();
        loop->runInLoop(std::bind(print, loop));
        CurrentThread::sleepUsec(100 * 1000);
    }
    {
        EventLoopThread elt3;
        EventLoop *loop = elt3.startLoop();
        loop->runInLoop(std::bind(quit, loop));
        CurrentThread::sleepUsec(500 * 1000);
    }
    return 0;
}