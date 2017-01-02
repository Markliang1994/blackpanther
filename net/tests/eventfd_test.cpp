//
// Created by mark on 1/2/17.
//

#include <blackpanther/net/EventLoop.h>
#include <blackpanther/base/Thread.h>

using namespace blackpanther;
using namespace blackpanther::net;

EventLoop *g_loop;
void thread1(void){
    g_loop->wakeup();
}
int main(void){
    EventLoop loop;
    Thread t1(&thread1);
    t1.start();
    g_loop = &loop;
    g_loop->loop();
    return 0;
}