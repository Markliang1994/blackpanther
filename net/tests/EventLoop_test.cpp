//
// Created by mark on 16-12-7.
//

#include <blackpanther/net/EventLoop.h>
#include <blackpanther/base/Thread.h>

using namespace blackpanther;

net::EventLoop g_loop;

void threadFunc(void){
    printf("my tid is: %d\n", CurrentThread::tid());
    g_loop.loop();
}
int main(){
    Thread t1(threadFunc);
    t1.start();
    t1.join();
    return 0;
}