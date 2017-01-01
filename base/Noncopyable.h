//
// Created by mark on 11/24/16.
//

#ifndef BLACKPANTHER_BASE_NONCOPYABLE_H
#define BLACKPANTHER_BASE_NONCOPYABLE_H

namespace blackpanther {

    class Noncopyable {
    protected:
        Noncopyable() = default;

        ~Noncopyable() = default;

    private:
        Noncopyable(const Noncopyable&) = delete;

        void operator=(const Noncopyable&) = delete;
    };
}

#endif //BLACKPANTHER_NONCOPYABLE_H
