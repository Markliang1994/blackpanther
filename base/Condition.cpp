//
// Created by mark on 11/18/16.
//

#include "Condition.h"

#include <time.h>
#include <errno.h>
#include <stdint.h>

bool blackpanther::Condition::waitForSeconds(double seconds) {
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);

    const int64_t kNanoSecondsPerSecond = 1e9;
    int64_t  nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

    abstime.tv_sec += static_cast<time_t>(abstime.tv_nsec + nanoseconds);
    abstime.tv_nsec = static_cast<long>(abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond;

    return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}
