//
// Created by mark on 11/24/16.
//

#ifndef BLACKPANTHER_BASE_NONCOPYABLE_H
#define BLACKPANTHER_BASE_NONCOPYABLE_H

namespace blackpanther {

    class noncopyable {
    protected:
        noncopyable() = default;

        ~noncopyable() = default;

    private:
        noncopyable(const noncopyable &) = delete;

        void operator=(const noncopyable &) = delete;
    };
}

#endif //BLACKPANTHER_NONCOPYABLE_H
