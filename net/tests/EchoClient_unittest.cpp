//
// Created by mark on 1/1/17.
//
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/InetAddress.h>
#include <blackpanther/net/TcpClient.h>

using namespace blackpanther;
using namespace blackpanther::net;

EventLoop *g_loop;
int main(){
    EventLoop loop;
    g_loop = &loop;
    InetAddress peeraddr;
    TcpClient client1(g_loop, peeraddr, "client1");
    g_loop->loop();
    return 0;
}
