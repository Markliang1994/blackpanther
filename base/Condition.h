//
// Created by mark on 11/18/16.
//

#ifndef BLACKPANTHER_BASE_CONDITION_H
#define BLACKPANTHER_BASE_CONDITION_H

#include <boost/noncopyable.hpp>
#include <pthread.h>

#include "Mutex.h"

namespace blackpanther{
    class Condition : boost::noncopyable{
    public:
        explicit Condition(MutexLock &mutex)
        :mutex_(mutex){
            MCHECK(pthread_cond_init(&pcond_, nullptr));
        }

        ~Condition(){
            MCHECK(pthread_cond_destroy(&pcond_));
        }

        void wait(){
            MutexLock::UnassignGuard ug(mutex_);
            MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
        }

        bool waitForSeconds(double seconds);

        void notify(){
            MCHECK(pthread_cond_signal(&pcond_));
        }

        void notifyAll(){
            MCHECK(pthread_cond_broadcast(&pcond_));
        }
    private:
        MutexLock &mutex_;
        pthread_cond_t pcond_;
    };
}
#endif //PRODUCERCONSUMER_CONDITION_H
