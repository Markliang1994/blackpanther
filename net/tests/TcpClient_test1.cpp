//
// Created by mark on 16-12-21.
//
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/TcpClient.h>
#include <blackpanther/base/Logging.h>

using namespace blackpanther;
using namespace blackpanther::net;

TcpClient *g_client;
EventLoop *g_loop;

void timeout(){
    LOG_INFO << "time out";
    g_client->stop();
}

int main(){
    EventLoop loop;
    g_loop = &loop;
    InetAddress serveraddr("127.0.0.1", 2); // no server
    TcpClient client(&loop, serveraddr, "TcpClient");
    g_client = &client;
    loop.runAfter(0.0, timeout);
    loop.runAfter(10.0, std::bind(&EventLoop::quit, &loop));
    client.connect();
    CurrentThread::sleepUsec(100 * 1000);
    loop.loop();
    return 0;
}
