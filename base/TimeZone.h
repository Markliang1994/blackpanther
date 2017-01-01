//
// Created by mark on 16-12-1.
//

#ifndef BLACKPANTHER_BASE_TIMEZONE_H
#define BLACKPANTHER_BASE_TIMEZONE_H

#include <blackpanther/base/Copyable.h>
#include <memory>
#include <time.h>

namespace blackpanther{
    class TimeZone : Copyable{
    public:
        explicit TimeZone(const char *zonefile);
        TimeZone(int eastOfUtc, const char *tzname);
        TimeZone(){ }

        bool valid() const {
            return static_cast<bool>(data_);
        }

        struct tm toLocalTime(time_t secondsSinceEpoch) const;
        time_t fromLocalTime(const struct tm&)const;

        static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);
        static time_t fromUtcTime(const struct tm&);
        static time_t fromUtcTime(int year, int month, int day, int hour, int minute, int seconds);

        struct Data;
    private:
        std::shared_ptr<Data> data_;
    };
}
#endif //BLACKPANTHER_TIMEZONE_H
