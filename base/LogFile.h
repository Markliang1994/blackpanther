//
// Created by mark on 16-12-3.
//

#ifndef BLACKPANTHER_BASE_LOGFILE_H
#define BLACKPANTHER_BASE_LOGFILE_H

#include <blackpanther/base/Mutex.h>

#include <memory>
#include "noncopyable.h"

namespace blackpanther{
    namespace FileUtil {
        class AppendFile;
    }
    class LogFile : noncopyable{
    public:
        LogFile(const std::string &basename, size_t rollSize,
                bool threadSafe = true,
                int flushInterval = 3,
                int checkEveryN = 1024);
        ~LogFile();

        void append(const char *logine, int len);
        void flush();
        void rollFile();

    private:
        void apeend_unlocked(const char *logline, int len);

        static std::string getLogFileName(const std::string &basename, time_t *now);

        const std::string basename_;
        const size_t rollSize_;
        const int flushInterval_;
        int checkEveryN_;

        int count_;

        std::unique_ptr<MutexLock> mutex_;
        time_t startOfPeriod_;
        time_t lastRoll_;
        time_t lastFlush_;
        std::unique_ptr<FileUtil::AppendFile> file_;
    };
}
}
#endif //BLACKPANTHER_LOGFILE_H
