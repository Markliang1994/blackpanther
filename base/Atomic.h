//
// Created by mark on 11/19/16.
//

#ifndef BLACKPANTHER_BASE_ATOMIC_H
#define BLACKPANTHER_BASE_ATOMIC_H

#include <boost/core/noncopyable.hpp>

#include <iostream>
#include <stdint.h>

namespace blackpanther{
    namespace detail{
        template <typename T>
        class AtomicInteger : boost::noncopyable{
        public:
            T get(){
#ifdef __GNUC__
                #if __GNUC__ > 4 || \
                    (__GNUC__ == 4 && __GNUC_MINOR__ >= 7 && __GNUC_PATCHLEVEL___ >= 0)
                        return __atomic_load_n(&value_, __ATOMIC_SEQ_CST);
                #else
                     return __sync_val_compare_and_swap(&value_, 0, 0);
                #endif
#else
                return __sync_val_compare_and_swap(&value_, 0, 0);
#endif
            }

            T getAndAdd(T x){
#ifdef __GNUC__
                #if __GNUC__ > 4 || \
                (__GNUC__ == 4 && __GNUC_MINOR__ >= 7 && __GNUC_PATCHLEVEL___ >= 0)
                    return __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST);
                #else
                    return __sync_fetch_and_add(&value_, x);
                #endif
#else
                return __sync_fetch_and_add(&value_, x);
#endif
            }

            T addAndGet(T x){

                return getAndAdd(x) + x;
            }

            T incrementAndGet(){
                return addAndGet(1);
            }


            T decrementAndGet(){
                return addAndGet(-1);
            }

            void add(T x){
                getAndAdd(x);
            }

            void increment(){
                incrementAndGet();
            }

            void decrement(){
                decrementAndGet();
            }

            T getAndSet(T newValue){
#ifdef __GNUC__
                #if __GNUC__ > 4 || \
                    (__GNUC__ == 4 && __GNUC_MINOR__ >= 7 && __GNUC_PATCHLEVEL___ >= 0)
                     __atomic_store_n(&value_, newValue, __ATOMIC_SEQ_CST);
                     return newValue;
                #else
                    return __sync_lock_test_and_set(&value_, newValue);
                #endif
#else
                return __sync_lock_test_and_set(&value_, newValue);
#endif
            }

        private:
            volatile T value_;

        };
    }
    typedef detail::AtomicInteger<int32_t> AtomicInt32;
    typedef detail::AtomicInteger<int64_t> AtomicInt64;
}
#endif //BLACKPANTHER_ATOMIC_H
