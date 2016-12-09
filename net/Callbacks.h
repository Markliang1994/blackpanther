//
// Created by mark on 16-12-9.
//

#ifndef BLACKPANTHER_NET_CALLBACKS_H
#define BLACKPANTHER_NET_CALLBACKS_H

#include <blackpanther/base/Timestamp.h>

#include <functional>
#include <memory>

namespace blackpanther{
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;

    template <typename T>
    inline T* getPointer(const std::shared_ptr<T> &ptr){
        return ptr.get();
    }

    template <typename T>
    inline T* getPointer(const std::unique_ptr<T> &ptr){
        return ptr.get();
    }

    namespace net{
        class Buffer;
        typedef std::function<void()> TimerCallback;
    }
}
#endif //BLACKPANTHER_CALLBACKS_H
