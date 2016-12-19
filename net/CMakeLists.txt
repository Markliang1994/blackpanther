set(net_srcs
        EventLoop.cpp
        Channel.cpp
        InetAddress.cpp
        SocketsOps.cpp
        Socket.cpp
        TimerQueue.cpp
        Timer.cpp
        Poller.cpp
        poller/DefaultPoller.cpp
        poller/EPollPoller.cpp
        poller/PollPoller.cpp
        )
add_library(blackpanther_net ${net_srcs})
target_link_libraries(blackpanther_net blackpanther_base)

set(net_headers
        Channel.h
        EventLoop.h
        InetAddress.h
        Endian.h
        SocketsOps.h
        Socket.h
        TimerQueue.h
        Timer.h
        TimerId.h
        Poller.h
        poller/EPollPoller.h
        poller/PollPoller.h)