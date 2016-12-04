//
// Created by mark on 16-12-1.
//

#include <blackpanther/base/LogStream.h>

#include <algorithm>

using namespace blackpanther;
using namespace blackpanther::detail;

namespace blackpanther{
    namespace detail{
        const char digits[] = "9876543210123456789";
        const char *zero = digits + 9;
        static_assert(sizeof(digits) == 20, "wrong number of digits");

        const char digitsHex[] = "0123456789ABCDEF";
        static_assert(sizeof(digitsHex) == 17, "wrong number of digitsHex");

        // Efficient Integer to String Conversions, By Matthew Wilson
        template <typename  T>
        size_t convert(char buf[], T value){
            T i = value;
            char *p = buf;

            do{
                int lsd = static_cast<int>(i%10);
                i /= 10;
                *p++ = zero[lsd];
            }while(i!=0);

            if(value < 0)
                *p++ = '-';

            *p = '\0';
            std::reverse(buf, p);
            return p -buf;
        }

        template <typename T>
        size_t convertHex(char buf[], uintptr_t value){
            uintptr_t  i = value;
            char *p = buf;
            do{
                int lsd = static_cast<int>(i%16);
                i /= 16;
                *p++ = digitsHex[lsd];
            }while(i != 0);

            *p = '\0';
            std::reverse(buf, p);

            return p - buf;
        }

        template class FixedBuffer<kSmallBuffer>;
        template class FixedBuffer<kLargeBuffer>;
    }
}

template <int SIZE>
const char *FixedBuffer<SIZE>::debugString(){
    *cur_ = '\0';
    return data_;
}


template <int SIZE>
void FixedBuffer<SIZE>::cookieStart() {

}

template <int SIZE>
void FixedBuffer<SIZE>::cookieEnd() {

}

template <typename T>
void LogStream::formatInteger(T v) {
    if(buffer_.avail() >= kMaxNumericSize){
        size_t len = convert(buffer_.current(), v);
        buffer_.add(len);
    }
}

void LogStream::staticCheck() {
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10, "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10, "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10, "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10, "kMaxNumericSize is large enough");
}

LogStream&  LogStream::operator <<(short v){
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator <<(unsigned short v) {
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator <<(int v){
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int) {
    return *this;
}

template <typename  T>
fmt::fmt(const char *f, T val) {
    static_assert(std::is_arithmetic<T>::value == true, "Muse be a arithmetic type.");

    length_ = snprintf(buf_, sizeof(buf_), f, val);
    assert(static_cast<size_t>(length_) < sizeof(buf_));
}

template fmt::fmt(const char* f, char);

template fmt::fmt(const char* f, short);
template fmt::fmt(const char* f, unsigned short);
template fmt::fmt(const char* f, int);
template fmt::fmt(const char* f, unsigned int);
template fmt::fmt(const char* f, long);
template fmt::fmt(const char* f, unsigned long);
template fmt::fmt(const char* f, long long);
template fmt::fmt(const char* f, unsigned long long);

template fmt::fmt(const char* f, float);
template fmt::fmt(const char* f, double);
