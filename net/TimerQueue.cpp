//
// Created by mark on 16-12-8.
//

#include <blackpanther/net/TimerQueue.h>

#include <blackpanther/base/Logging.h>
#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/TimerId.h>

#include <functional>

#include <sys/timerfd.h>

namespace blackpanther{
    namespace net{
        namespace  detail{
            int createTimerfd(){
                int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                               TFD_NONBLOCK | TFD_CLOEXEC);

                if(timerfd < 0)
                    LOG_SYSFATAL << "Failed in timerfd_create";
                return timerfd;
            }

            struct timespec howmuchTimeFromNow(Timestamp when){
                int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();

                if(microseconds < 100)
                    microseconds = 100;

                struct timespec ts;
                ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
                ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSecond)*1000);
                return ts;
            }

            void readTimerfd(int timerfd, Timestamp now){
                uint64_t howmany;
                ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
                LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
                if(n != sizeof(howmany))
                    LOG_SYSERR << "TimerQueue::handleRead() reads " << n << "bytes instead of 8";
            }

            void resetTimerfd(int timerfd, Timestamp expiration){
                struct itimerspec newValue;
                struct itimerspec oldValue;

                bzero(&newValue, sizeof(newValue));
                bzero(&oldValue, sizeof(oldValue));
                newValue.it_value = howmuchTimeFromNow(expiration);
                int ret = ::timer_settime(timerfd, 0, &newValue, &oldValue);
                if(ret){
                    LOG_SYSERR << "timerfd_settime()";
                }
            }
        }
    }
}

using namespace blackpanther;
using namespace blackpanther::net;
using namespace blackpanther::net::detail;

TimerQueue::TimerQueue(EventLoop *loop)
    : loop_(loop),
     timerfd_(createTimerfd()),
     timerfdChannel_(loop, timerfd_),
     timers_(),
     callingExpiredTimers_(false){
    timerfdChannel_.setReadCallback(
            std::bind(&TimerQueue::handleRead, this));
    timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue() {
    timerfdChannel_.disableAll();
    timerfdChannel_.remove();
    ::close(timerfd_);

    for(TimerList::iterator it = timers_.begin(); it != timers_.end(); ++it)
        delete it->second;
}

Timer TimerQueue::addTimer(const TimerCallback &cb,
                            Timestamp when,
                            double interval){
    Timer *timer = new Timer(cb, when, interval);
}