#include <blackpanther/net/TcpClient.h>

#include <blackpanther/base/Logging.h>
#include <blackpanther/base/Thread.h>
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/InetAddress.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include <functional>
#include <utility>

#include <stdio.h>
#include <unistd.h>

using namespace blackpanther;
using namespace blackpanther::net;

int numThreads = 0;
class EchoClient;
boost::ptr_vector<EchoClient> clients;
int current = 0;

class EchoClient : Noncopyable
{
public:
    EchoClient(EventLoop* loop, const InetAddress& listenAddr, const std::string& id)
            : loop_(loop),
              client_(loop, listenAddr, "EchoClient"+id)
    {
        client_.setConnectionCallback(
                std::bind(&EchoClient::onConnection, this, std::placeholders::_1));
        client_.setMessageCallback(
                std::bind(&EchoClient::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        //client_.enableRetry();
    }

    void connect()
    {
        client_.connect();
    }
    void stop(){
        client_.stop();
    }

private:
    void onConnection(const TcpConnectionPtr& conn)
    {
        LOG_TRACE << conn->localAddress().toIpPort() << " -> "
                  << conn->peerAddress().toIpPort() << " is "
                  << (conn->connected() ? "UP" : "DOWN");

        if (conn->connected())
        {
            ++current;
            if (boost::implicit_cast<size_t>(current) < clients.size())
            {
                clients[current].connect();
            }
            LOG_INFO << "*** connected " << current;
        }
        conn->send("world\n");
    }

    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
    {
        std::string msg(buf->retrieveAllAsString());
        LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toString();
        if (msg == "quit\n")
        {
            conn->send("bye\n");
            conn->shutdown();
        }
        else if (msg == "shutdown\n")
        {
            loop_->quit();
        }
        else
        {
            conn->send(msg);
        }
    }

    EventLoop* loop_;
    TcpClient client_;
};

int main(int argc, char* argv[])
{
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    if (argc > 1)
    {
        EventLoop loop;
        bool ipv6 = argc > 3;
        InetAddress serverAddr(argv[1], 1999, ipv6);

        int n = 1;
        if (argc > 2)
        {
            n = atoi(argv[2]);
        }

        clients.reserve(n);
        for (int i = 0; i < n; ++i)
        {
            char buf[32];
            snprintf(buf, sizeof buf, "%d", i+1);
            clients.push_back(new EchoClient(&loop, serverAddr, buf));
        }

        clients[current].connect();
        loop.loop();
    }
    else
    {
        printf("Usage: %s host_ip [current#]\n", argv[0]);
    }
}

