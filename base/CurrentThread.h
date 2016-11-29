//
// Created by mark on 11/18/16.
//

#ifndef BLACKPANTHER_BASE_CURRENTTHREAD_H
#define BLACKPANTHER_BASE_CURRENTTHREAD_H

#include <stdint.h>

namespace blackpanther{
    namespace CurrentThread{
        extern __thread int t_cachedTid;
        extern __thread char t_tidString[32];
        extern __thread int t_tidStringLength;
        extern __thread const char *t_threadName;

        void cacheTid();

        inline int tid(){
            if(__builtin_expect(t_cachedTid == 0, 0)){
                cacheTid();
            }
            return t_cachedTid;
        }

        inline int tidStringLength(){
            return t_tidStringLength;
        }

        inline const char* name(){
            return t_threadName;
        }

        bool isMainThread();

        void sleepUsec(int64_t usec);
    }
}
#endif //PRODUCERCONSUMER_CURRENTTHREAD_H
