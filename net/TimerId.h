//
// Created by mark on 16-12-9.
//

#ifndef BLACKPANTHER_NET_TIMERID_H
#define BLACKPANTHER_NET_TIMERID_H

#include <blackpanther/base/noncopyable.h>

namespace blackpanther{
    namespace net{
        class Timer;

        class TimerId : public blackpanther::noncopyable{
        public:
            TimerId()
                    : timer_(nullptr),
                     sequence_(0)
            {
            }
            TimerId(Timer *timer, int64_t seq)
                    : timer_(timer),
                      sequence_(seq){
            }

            friend class TimerQueue;

        private:
            Timer *timer_;
            int64_t sequence_;
        };
    }
}
#endif //BLACKPANTHER_TIMERID_H
