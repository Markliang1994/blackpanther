//
// Created by mark on 16-12-15.
//
#include <blackpanther/base/Logging.h>
#include <blackpanther/net/InetAddress.h>


#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

using std::string;

using blackpanther::net::InetAddress;

BOOST_AUTO_TEST_CASE(testInetAddress){
    InetAddress addr0(1234);
    BOOST_CHECK_EQUAL(addr0.toIp(), std::string("0.0.0.0"));
    BOOST_CHECK_EQUAL(addr0.toIpPort(), std::string("0.0.0.0:1234"));
    BOOST_CHECK_EQUAL(addr0.toPort(), 1234);

    InetAddress addr1(4321, true);
    BOOST_CHECK_EQUAL(addr1.toIp(), string("127.0.0.1"));
    BOOST_CHECK_EQUAL(addr1.toPort(), 4321);
    BOOST_CHECK_EQUAL(addr1.toIpPort(), string("127.0.0.1:4321"));

    InetAddress addr2("41.41.41.41", 8888);
    BOOST_CHECK_EQUAL(addr2.toIp(), string("41.41.41.41"));
    BOOST_CHECK_EQUAL(addr2.toIpPort(), string("41.41.41.41:8888"));
    BOOST_CHECK_EQUAL(addr2.toPort(), 8888);

    InetAddress addr3("255.254.253.252", 65535);
    BOOST_CHECK_EQUAL(addr3.toIp(), string("255.254.253.252"));
    BOOST_CHECK_EQUAL(addr3.toPort(), 65535);
    BOOST_CHECK_EQUAL(addr3.toIpPort(), string("255.254.253.252:65535"));
}

BOOST_AUTO_TEST_CASE(testInetAddressResolve){
    InetAddress addr(80);
    if(InetAddress::resolve("google.com", &addr)){
        LOG_INFO << "google.com resolved to " << addr.toIpPort();
    }
    else
        LOG_ERROR << "Unable to resolve google.com";
}

