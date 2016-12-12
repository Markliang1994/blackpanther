//
// Created by mark on 16-12-7.
//

#include <blackpanther/net/EventLoop.h>
#include <blackpanther/base/Thread.h>

using namespace blackpanther;
using namespace blackpanther::net;

void callback(){
    printf("callback(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
    EventLoop anotherLoop;
}

void threadFunc(void){
    printf("threadFunc(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
    assert(EventLoop::getEventLoopOfCurrentThread() == nullptr);
    EventLoop loop;
    assert(EventLoop::getEventLoopOfCurrentThread() == &loop);
    loop.runAfter(1.0, callback);
    loop.loop();
}
int main(){
    printf("main(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
    assert(EventLoop::getEventLoopOfCurrentThread() == nullptr);
    EventLoop loop;
    assert(EventLoop::getEventLoopOfCurrentThread() == &loop);

    Thread t1(threadFunc);
    t1.start();

    loop.loop();
    return 0;
}