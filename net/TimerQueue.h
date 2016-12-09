//
// Created by mark on 16-12-8.
//

#ifndef BLACKPANTHER_NET_TIMERQUEUE_H
#define BLACKPANTHER_NET_TIMERQUEUE_H

#include <blackpanther/base/noncopyable.h>
#include <blackpanther/base/Timestamp.h>

#include <blackpanther/net/Channel.h>
#include <blackpanther/net/TimerId.h>
#include <blackpanther/net/Callbacks.h>

#include <vector>
#include <set>

namespace blackpanther{
    namespace  net{
        class TimerQueue : noncopyable{
        public:
            TimerQueue(EventLoop *loop);
            ~TimerQueue();

            TimerId addTimer(const TimerCallback &cb,
                            Timestamp when,
                            double interval);

            TimerId addTimer(TimerCallback &&cb,
                            Timestamp when,
                            double interval);

            void cancel(TimerId timerId);

        private:
            typedef std::pair<Timestamp, Timer*> Entry;
            typedef std::set<Entry> TimerList;
            typedef std::pair<Timer*, int64_t> ActiveTimer;
            typedef std::set<ActiveTimer> ActiveTimerSet;

            void addTimerInLoop(Timer *timer);
            void cancelInLoop(TimerId timerId);

            void handleRead();

            std::vector<Entry> getExpired(Timestamp now);
            void reset(const std::vector<Entry&> expired, Timestamp now);

            bool insert(Timer *timer);

            EventLoop *loop_;
            const int timerfd_;
            Channel timerfdChannel_;

            TimerList timers_;

            ActiveTimerSet activeTimers_;
            bool callingExpiredTimers_;
            ActiveTimerSet cancelingTimers_;
        };
    }
}
#endif //BLACKPANTHER_TIMERQUEUE_H
