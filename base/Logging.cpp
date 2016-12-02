//
// Created by mark on 16-12-1.
//

#include <blackpanther/base/Logging.h>
#include <blackpanther/base/CurrentThread.h>
#include <blackpanther/base/Timestamp.h>
#include <blackpanther/base/TimeZone.h>

namespace blackpanther{
    __thread char t_errnobuf[512];
    __thread char t_time[32];
    __thread time_t t_lastSecond;

    const char *sterror_tl(int savedErrno){
        return strerror_r(savedErrno, t_errnobuf, sizeof(t_errnobuf));
    }

    Logger::LogLevel initLogLevel(){
        if(::getenv("BLACKPANTHER_LOG_TRACE"))
            return Logger::TRACE;
        else if(::getenv("BLACKPANTHER_LOG_DEBUG"))
            return Logger::DEBUG;
        else
            return Logger::INFO;
    }

    Logger::LogLevel g_logLevel = initLogLevel();

    const char *LogLevelName[Logger::NUM_LOG_LEVELS] ={
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"
    };

    class T{
    public:
        T(const char *str, unsigned len):
                str_(str),len_(len){
            assert(strlen(str) == len_);
        }

        const char *str_;
        const unsigned len_;
    };

    inline LogStream& operator <<(LogStream &s, T v){
        s.append(v.str_, v.len_);
        return s;
    }

    inline LogStream& operator <<(LogStream &s, const Logger::SourceFile &v){
        s.append(v.data_, v.size_);
        return s;
    }

    void defaultOutput(const char *msg, int len){
        size_t n = fwrite(msg, 1, len, stdout);
        (void)n;

    }

    void defaultFlush(){
        fflush(stdout);
    }
    Logger::OutputFunc g_output = defaultOutput;
    Logger::FlushFunc  g_flush = defaultFlush;
    TimeZone g_logTimeZone;
}

using namespace blackpanther;

Logger::Impl::Impl(LogLevel level, int old_errno, const SourceFile &file, int line)
    :time_(Timestamp::now()),
    stream_(),
    level_(level),
    line_(line),
    basename_(file){
}

Logger::Impl::~Impl(){

}

void Logger::Impl::formatTime() {
    //TODO

}

void Logger::Impl::finish() {
    //TODO
}

Logger::Logger(SourceFile file, int line)
    :impl_(INFO, 0, file, line){

}

Logger::Logger(SourceFile file, int line, LogLevel level, const char *func):
    impl_(level, 0, file, line){
    impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level):
    impl_(level, 0, file, line){

}

Logger::Logger(SourceFile file, int line, bool toAbort)
        :impl_(toAbort? FATAL:ERROR, errno, file, line){

}

Logger::~Logger() {
    //TODO
}

void Logger::setLogLevel(Logger::LogLevel level) {
    g_logLevel = level;
}

void Logger::setOutput(OutputFunc out) {
    g_output = out;
}

void Logger::setFlush(FlushFunc flush){
    g_flush = flush;
}

void Logger::setTimeZone(const TimeZone &tz) {
    g_logTimeZone = tz;
}