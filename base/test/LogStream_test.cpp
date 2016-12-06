//
// Created by mark on 16-12-5.
//
#include <blackpanther/base/LogStream.h>
#include <limits>
#include <stdint.h>

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testLogStreamBooleans)
{
    blackpanther::LogStream os;
    const blackpanther::LogStream::Buffer &buf = os.buffer();
    BOOST_CHECK_EQUAL(buf.toString(), std::string(""));
    os << true;

    BOOST_CHECK_EQUAL(buf.toString(), std::string("1"));
    os << '\n';
    BOOST_CHECK_EQUAL(buf.toString(), std::string("1\n"));
    os << false;
    BOOST_CHECK_EQUAL(buf.toString(), std::string("1\n0"));
}

BOOST_AUTO_TEST_CASE(testLogStreaIntgers){
    blackpanther::LogStream os;
    const blackpanther::LogStream::Buffer &buf = os.buffer();
    BOOST_CHECK_EQUAL(buf.toString(), std::string(""));
    os << 1;
    BOOST_CHECK_EQUAL(buf.toString(), std::string("1"));
    os << 0;
    BOOST_CHECK_EQUAL(buf.toString(), std::string("10"));
    os << -1;
    BOOST_CHECK_EQUAL(buf.toString(), std::string("10-1"));
    os.resetBuffer();

    os << 0 <<" " << 123 << 'x' << 0x64;
    BOOST_CHECK_EQUAL(buf.toString(), std::string("0 123x100"));
}

BOOST_AUTO_TEST_CASE(testLogStreamIntegerLimits){
    blackpanther::LogStream os;
    const blackpanther::LogStream::Buffer &buf = os.buffer();
    os << -2147483647;
    BOOST_CHECK_EQUAL(buf.toString(), std::string("-2147483647"));
    os << static_cast<int>(-2147483647-1);
    BOOST_CHECK_EQUAL(buf.toString(), std::string("-2147483647-2147483648"));
    os << ' ';
    os << 2147483647;
    BOOST_CHECK_EQUAL(buf.toString(), std::string("-2147483647-2147483648 2147483647"));
    os.resetBuffer();

    os << std::numeric_limits<int16_t>::min();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("-32768"));
    os.resetBuffer();

    os << std::numeric_limits<int16_t>::max();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("32767"));
    os.resetBuffer();

    os << std::numeric_limits<uint16_t>::min();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("0"));
    os.resetBuffer();

    os << std::numeric_limits<uint16_t>::max();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("65535"));
    os.resetBuffer();

    os << std::numeric_limits<int32_t>::min();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("-2147483648"));
    os.resetBuffer();

    os << std::numeric_limits<int32_t>::max();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("2147483647"));
    os.resetBuffer();

    os << std::numeric_limits<uint32_t>::min();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("0"));
    os.resetBuffer();

    os << std::numeric_limits<uint32_t>::max();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("4294967295"));
    os.resetBuffer();

    os << std::numeric_limits<int64_t>::min();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("-9223372036854775808"));
    os.resetBuffer();

    os << std::numeric_limits<int64_t>::max();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("9223372036854775807"));
    os.resetBuffer();

    os << std::numeric_limits<uint64_t>::min();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("0"));
    os.resetBuffer();

    os << std::numeric_limits<uint64_t>::max();
    BOOST_CHECK_EQUAL(buf.toString(), std::string("18446744073709551615"));
    os.resetBuffer();

    int16_t a = 0;
    int32_t b = 0;
    int64_t c = 0;
    os << a;
    os << b;
    os << c;
    BOOST_CHECK_EQUAL(buf.toString(), std::string("000"));
}

