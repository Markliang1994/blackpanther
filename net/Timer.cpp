//
// Created by mark on 16-12-9.
//

#include <blackpanther/net/Timer.h>

using namespace blackpanther;
using namespace blackpanther::net;

AtomicInt64 Timer::s_numCreated_;

void Timer::restart(Timestamp now) {
    if(repeat_){
        expiration_ = addTime(now, interval_);
    }
    else{
        expiration_ = Timestamp::invalid();
    }
}