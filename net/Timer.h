//
// Created by mark on 16-12-9.
//

#ifndef BLACKPANTHER_NET_TIMER_H
#define BLACKPANTHER_NET_TIMER_H

#include <blackpanther/base/Atomic.h>
#include <blackpanther/base/Timestamp.h>
#include <blackpanther/net/Callbacks.h>

namespace blackpanther{
    namespace net{
        class Timer : noncopyable{
        public:
            Timer(const TimerCallback &cb, Timestamp when, double interval)
                    : callback_(cb),
                      expiration_(when),
                      interval_(interval),
                      repeat_(interval_ > 0.0),
                      sequence_(s_numCreated_.incrementAndGet()){
            }

            Timer(TimerCallback &&cb, Timestamp when, double interval)
                    : callback_(std::move(cb)),
                      expiration_(when),
                      interval_(interval),
                      repeat_(interval_ > 0.0),
                      sequence_(s_numCreated_.incrementAndGet()){
            }

            void run() const{
                callback_();
            }

            Timestamp expiration() const { return expiration_; }
            bool repeat() const { return repeat_; }
            int64_t sequence_() const { return sequence_; }
            void start(Timestamp now);

            static int64_t numCreated() const { return s_numCreated_.get(); }

        private:
            const TimerCallback callback_;
            Timestamp expiration_;
            const double interval_;
            const bool repeat_;
            const int64_t sequence_;

            static AtomicInt64 s_numCreated_;
        };
    }
}
#endif //BLACKPANTHER_TIMER_H
