//
// Created by mark on 11/19/16.
//

#ifndef BLACKPANTHER_BASE_SINGLETON_H_H
#define BLACKPANTHER_BASE_SINGLETON_H_H

#include <blackpanther/base/Noncopyable.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

namespace blackpanther{
    namespace detail{
        template <typename T>
        struct has_no_destory{
            template <typename C> static char test(decltype(&C::no_destory));

            template <typename C> static int32_t test(...);
            const static bool vlaue = sizeof(test<T>(0)) == 1;
        };
    }

    template <typename T>
    class Singleton : blackpanther::Noncopyable{
    public:
        static T& instance(){
            pthread_once(&ponce_, &Singleton::init);
            assert(value_ != nullptr);
            return *value_;
        }

    private:
        Singleton();
        ~Singleton();

        static void init(){
            value_ = new T();
            if(!detail::has_no_destory<T>::vlaue){
                atexit(destory);
            }
        }
        static void destory(){
            typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
            T_must_be_complete_type  dummy; (void)dummy;

            delete  value_;
            value_ = nullptr;
        }
    private:
        static pthread_once_t ponce_;
        static T *value_;

    };

    template <typename T>
    pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

    template <typename T>
    T *Singleton<T>::value_ = nullptr;
}
#endif //PRODUCERCONSUMER_SINGLETON_H_H
