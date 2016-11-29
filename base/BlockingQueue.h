//
// Created by mark on 16-11-29.
//

#ifndef BLACKPANTHER_BASE_BLOCKINGQUEUE_H
#define BLACKPANTHER_BASE_BLOCKINGQUEUE_H

#include <blackpanther/base/Condition.h>
#include <blackpanther/base/Mutex.h>

#include <blackpanther/base/noncopyable.h>

#include <deque>
#include <assert.h>

#include <stdio.h>

namespace  blackpanther{
    template <typename T>
    class BlockingQueue : blackpanther::noncopyable{
    public:
        BlockingQueue()
                : mutex_(), notEmpty_(mutex_),
                  queue_(){

        }

        void put(const T &x){
            MutexLockGuard lock(mutex_);
            queue_.push_back(x);
            notEmpty_.notify();

        }

        void put(T &&x){
            MutexLockGuard lock(mutex_);
            queue_.push_back(std::move(x));
            notEmpty_.notify();
        }

        T take(){
            MutexLockGuard lock(mutex_);
            while(queue_.empty()){
                notEmpty_.wait();
            }
            assert(!queue_.empty());
            T front(std::move(queue_.front()));
            queue_.pop_front();
            return front;
        }

        size_t size() const {
            MutexLockGuard lock(mutex_);
            return queue_.size();
        }
    private:
        mutable MutexLock mutex_;
        Condition notEmpty_;
        std::deque<T> queue_;
    };
}
#endif //BLACKPANTHER_BLOCKINGQUEUE_H
