//
// Created by mark on 16-12-1.
//

#ifndef BLACKPANTHER_BASE_LOGGING_H
#define BLACKPANTHER_BASE_LOGGING_H

#include <blackpanther/base/LogStream.h>
#include <blackpanther/base/Timestamp.h>

#include <string.h>

namespace blackpanther{

    class TimeZone;
    class Logger{
    public:
        enum LogLevel{
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_LOG_LEVELS,
        };

        class SourceFile{
        public:
            template <int N>
            inline SourceFile(const char (&arr)[N]):
                    data_(arr), size_(N-1){
                const char *slash = strrchr(data_, '/');
                if(slash){
                    data_ = slash + 1;
                    size_ -= static_cast<int>(data_ - arr);
                }
            }

            explicit SourceFile(const char *filename):
                    data_(filename){
                const char *slash = strrchr(filename, '/');
                if(slash){
                    data_ = slash + 1;
                }
                size_ = static_cast<int>(strlen(data_));
            }
            const char *data_;
            int size_;
        };

        Logger(SourceFile file, int line);
        Logger(SourceFile file, LogLevel level);
        Logger(SourceFile file, int line, LogLevel level, const char *func);
        Logger(SourceFile file, int line, LogLevel level);
        Logger(SourceFile file, int line, bool toAbort);
        ~Logger();

        LogStream& stream() { return impl_.stream_; }

        static LogLevel logLevel();
        static void setLogLevel(LogLevel);

        typedef void (*OutputFunc)(const char *msg, int line);
        typedef void (*FlushFunc)();

        static void setOutput(OutputFunc);
        static void setFlush(FlushFunc);
        static void setTimeZone(const TimeZone &tz);

    private:
        class Impl{
        public:
            typedef Logger::LogLevel LogLevel;
            Impl(LogLevel level, int old_errno, const SourceFile &file, int line);
            ~Impl();
            void formatTime();
            void finish();

            Timestamp time_;
            LogStream stream_;
            LogLevel level_;
            int line_;
            SourceFile basename_;
        };

        Impl impl_;
    };
    extern Logger::LogLevel g_logLevel;

    inline Logger::LogLevel Logger::logLevel() {
        return g_logLevel;
    }

#define LOG_TRACE if(blackpanther::Logger::logLevel() <= blackpanther::Logger::TRACE) \
        blackpanther::Logger(__FILE__, __LINE__, blackpanther::Logger::TRACE, __func__).stream()

#define LOG_DEBUG if(blackpanther::Logger::logLevel() <= blackpanther::Logger::DEBUG) \
        blackpanther::Logger(__FILE__, __LINE__, blackpanther::Logger::DEBUG, __func__).stream()

#define LOG_INFO if(blackpanther::Logger::logLevel() <= blackpanther::Logger::INFO) \
        blackpanther::Logger(__FILE__, __LINE__).stream()

#define LOG_WARN blackpanther::Logger(__FILE__, __LINE__, blackpanther::Logger::WARN).stream();
#define LOG_ERROR blackpanther::Logger(__FILE__, __LINE__, blackpanther::Logger::ERROR).stream();
#define LOG_FATAL blackpanther::Logger(__FILE__, __LINE__, blackpanther::Logger::FATAL).stream()
#define LOG_SYSERR blackpanther::Logger(__FILE__, __LINE__, flase).stream()
#define LOG_SYSFATAL blackpanther::Logger(__FILE__, __LINE__, true).stream()

    const char *strerror_tl(int savedErrno);

#define CHECK_NOTNULL(val) \
    ::blackpanther::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

    template <typename T>
    T *CheckNotNull(Logger::SourceFile file, int line, const char *names, T *ptr){
        if(ptr == nullptr)
            //Logger(file, line, Logger::FATAL).stream() << names;
            return nullptr;
        return ptr;
    }
}

#endif //BLACKPANTHER_LOGGING_H
