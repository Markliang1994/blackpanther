//
// Created by mark on 16-12-15.
//
#include <blackpanther/base/Logging.h>
#include <blackpanther/net/InetAddress.h>


#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

using blackpanther::net::InetAddress;

BOOST_AUTO_TEST_CASE(testInetAddress){
    InetAddress addr0(1234);
}
