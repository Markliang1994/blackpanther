//
// Created by mark on 16-11-29.
//

#ifndef BLACKPANTHER_BASE_BOUNDEDBLOCKINGQUEUE_H
#define BLACKPANTHER_BASE_BOUNDEDBLOCKINGQUEUE_H

#include <blackpanther/base/Condition.h>
#include <blackpanther/base/Mutex.h>
#include <blackpanther/base/noncopyable.h>

#include <boost/circular_buffer.hpp>
#include <assert.h>

namespace blackpanther{
    template <typename  T>
    class BoundedBlockingQueue : blackpanther::noncopyable{
    public:
        explicit BoundedBlockingQueue(int maxSize):
                mutex_(), notEmpty_(mutex_), notFull_(mutex_),
            queue_(maxSize){

        }
    private:
        mutable MutexLock mutex_;
        Condition notEmpty_;
        Condition notFull_;
        boost::circular_buffer<T> queue_;
    };
}
#endif //BLACKPANTHER_BOUNDEDBLOCKINGQUEUE_H
