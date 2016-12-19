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

BOOST_AUTO_TEST_CASE(testBufferInsideGrow){
        Buffer buf;
        buf.append(std::string(800, 'y'));
        BOOST_CHECK_EQUAL(buf.readableBytes(), 800);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 800);

        buf.retrieve(500);
        BOOST_CHECK_EQUAL(buf.readableBytes(), 300);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 800);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + 500);

        buf.append(std::string(300, 'z'));
        BOOST_CHECK_EQUAL(buf.readableBytes(), 600);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 600);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
}

BOOST_AUTO_TEST_CASE(testBufferShrink){
        Buffer buf;
        buf.append(std::string(2000, 'x'));
        BOOST_CHECK_EQUAL(buf.readableBytes(), 2000);
        BOOST_CHECK_EQUAL(buf.writableBytes(), 0);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

        buf.retrieve(1500);
        BOOST_CHECK_EQUAL(buf.readableBytes(), 500);
        BOOST_CHECK_EQUAL(buf.writableBytes(), 0);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + 1500);

        buf.shrink(0);
        BOOST_CHECK_EQUAL(buf.readableBytes(), 500);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 500);
        BOOST_CHECK_EQUAL(buf.retrieveAllAsString(), std::string(500, 'x'));
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
}

BOOST_AUTO_TEST_CASE(testBufferPrepend){
        Buffer buf;
        buf.append(std::string(200, 'f'));
        BOOST_CHECK_EQUAL(buf.readableBytes(), 200);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 200);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

        int x = 0;
        buf.prepend(&x, sizeof(x));
        BOOST_CHECK_EQUAL(buf.readableBytes(), 204);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 200);
        BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend - 4);
}

BOOST_AUTO_TEST_CASE(testBufferReadInt){
        Buffer buf;
        buf.append("HTTP");

        BOOST_CHECK_EQUAL(buf.readableBytes(), 4);
        BOOST_CHECK_EQUAL(buf.peekInt8(), 'H');
        int top16 = buf.peekInt16();
        BOOST_CHECK_EQUAL(top16, 'H'*256 + 'T');
        BOOST_CHECK_EQUAL(buf.peekInt32(), top16*65536 + 'T'*256 + 'P');

        BOOST_CHECK_EQUAL(buf.readInt8(), 'H');
        BOOST_CHECK_EQUAL(buf.readInt16(), 'T'*256 + 'T');
        BOOST_CHECK_EQUAL(buf.readInt8(), 'P');
        BOOST_CHECK_EQUAL(buf.readableBytes(), 0);
        BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize);

        buf.appendInt8(-1);
        buf.appendInt16(-2);
        buf.appendInt32(-3);
        BOOST_CHECK_EQUAL(buf.readableBytes(), 7);
        BOOST_CHECK_EQUAL(buf.readInt8(), -1);
        BOOST_CHECK_EQUAL(buf.readInt16(), -2);
        BOOST_CHECK_EQUAL(buf.readInt32(), -3);
}