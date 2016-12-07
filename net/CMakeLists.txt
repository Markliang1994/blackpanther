set(net_srcs
        EventLoop.cpp
        )
add_library(blackpanther_net ${net_srcs})
target_link_libraries(blackpanther_net blackpanther_base)

set(net_headers
        EventLoop.h)