//
// Created by mark on 16-12-7.
//

#ifndef BLACKPANTHER_BASE_WEAKCALLBACK_H
#define BLACKPANTHER_BASE_WEAKCALLBACK_H

#include <functional>
#include <memory>

namespace blackpanther{
    template <typename T, typename ...args>
    class WeakCallback{
    public:
        WeakCallback(const std::weak_ptr<T> &object,
                     const std::function<void (T*, args...)> &function)
                : object_(object), function_(function){
        }

        void operator()(args&& ...arg_list) const{
            std::shared_ptr<T> ptr(object_.lock());
            if(ptr)
                function_(ptr.get());
        }

    private:
        std::weak_ptr<T> object_;
        std::function<void(T*)> function_;
    };
    template <typename T, typename ...args>
    WeakCallback<T> makeWeakCallback(const std::shared_ptr<T> &object, void (T::*function)(args...)){
        return WeakCallback<T>(object, function);
    }

    template <typename T, typename ...args>
    WeakCallback<T> makeWeakCallback(const std::shared_ptr<T> &object, void(T::*function)(args...)const){
        return WeakCallback<T>(object, function);
    }
}

#endif //BLACKPANTHER_WEAKCALLBACK_H
