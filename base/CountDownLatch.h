//
// Created by mark on 16-11-29.
//

#ifndef BLACKPANTHER_BASE_COUNTDOWNLATCH_H
#define BLACKPANTHER_BASE_COUNTDOWNLATCH_H

#include <blackpanther/base/Condition.h>
#include <blackpanther/base/Mutex.h>

#include <blackpanther/base/Noncopyable.h>

namespace blackpanther{
    class CountDownLatch : blackpanther::Noncopyable{
    public:
        explicit CountDownLatch(int count);

        void wait();

        void countDown();

        int getCount() const;

    private:
        mutable MutexLock mutex_;
        Condition condition_;
        int count_;
    };
}
#endif //BLACKPANTHER_COUNTDOWNLATCH_H
