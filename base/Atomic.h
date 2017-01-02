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
                        return __atomic_load_n(&value_, __ATOMIC_SEQ_CST);
            }

            T getAndAdd(T x){
                    return __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST);
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
                return __atomic_exchange_n(&value_, newValue, __ATOMIC_SEQ_CST);
            }

        private:
            volatile T value_;

        };
    }
    typedef detail::AtomicInteger<int32_t> AtomicInt32;
    typedef detail::AtomicInteger<int64_t> AtomicInt64;
}
#endif //BLACKPANTHER_ATOMIC_H
