//
// Created by mark on 16-12-7.
//

#ifndef BLACKPANTHER_BASE_WEAKCALLBACK_H
#define BLACKPANTHER_BASE_WEAKCALLBACK_H

#include <functional>
#include <memory>

namespace blackpanther{
    template <typename T, typename ...args>
    class WeakCallBack{
    public:
        WeakCallBack(const std::weak_ptr<T> &object,
                     const std::function<void (T*, args...)> &function)
                : object_(object), function_(function){
        }

    private:
        std::weak_ptr<T> object_;
        std::function<void(T*)> function_;
    };
}
#endif //BLACKPANTHER_WEAKCALLBACK_H
