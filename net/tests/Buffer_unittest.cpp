#include <blackpanther/net/Buffer.h>

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

using blackpanther::net::Buffer;

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testBufferAppendRetreive){
        Buffer buf;
        BOOST_CHECK_EQUAL(buf.readableBytes(), 0);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

        const std::string str(200, 'x');
        buf.append(str);
        BOOST_CHECK_EQUAL(buf.readableBytes(), str.size());
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - str.size());
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

        const std::string str2 = buf.retrieveAsString(50);
        BOOST_CHECK_EQUAL(str2.size(), 50);
        BOOST_CHECK_EQUAL(buf.readableBytes(), str.size() - str2.size());
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - str.size());
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + str2.size());
        BOOST_CHECK_EQUAL(str2, std::string(50, 'x'));

        buf.append(str);
        BOOST_CHECK_EQUAL(buf.readableBytes(), 2*str.size() - str2.size());
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 2*str.size());
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + str2.size());

        const std::string str3 = buf.retrieveAllAsString();
        BOOST_CHECK_EQUAL(str3.size(), 350);
        BOOST_CHECK_EQUAL(buf.readableBytes(), 0);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
        BOOST_CHECK_EQUAL(str3, std::string(350, 'x'));

}

BOOST_AUTO_TEST_CASE(testBufferGrow){
        Buffer buf;
        buf.append(std::string(400, 'y'));
        BOOST_CHECK_EQUAL(buf.readableBytes(), 400);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 400);

        buf.retrieve(50);
        BOOST_CHECK_EQUAL(buf.readableBytes(), 350);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 400);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + 50);

        buf.append(std::string(1000, 'z'));
        BOOST_CHECK_EQUAL(buf.readableBytes(), 1350);
        BOOST_CHECK_EQUAL(buf.writableBytes(), 0);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend+50);

        buf.retrieveAll();
        BOOST_CHECK_EQUAL(buf.readableBytes(), 0);
        BOOST_CHECK_EQUAL(buf.writableBytes(), 1400);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
}