//
// Created by mark on 1/1/17.
//
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/TcpServer.h>
#include <blackpanther/net/InetAddress.h>
#include <blackpanther/net/EventLoopThread.h>

using namespace blackpanther;
using namespace blackpanther::net;

int main(void){
    EventLoop *loop1;
    EventLoopThread elp1;
    loop1 = elp1.startLoop();
    InetAddress localaddr("127.0.0.1", 1994);
    TcpServer server1(loop1, localaddr, "server1");
    server1.start();
    return 0;
}