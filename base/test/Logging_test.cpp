//
// Created by mark on 16-12-2.
//

#include <blackpanther/base/Logging.h>
#include <blackpanther/base/LogFile.h>
#include <blackpanther/base/ThreadPool.h>
#include <blackpanther/base/TimeZone.h>

#include <stdio.h>

using namespace blackpanther;

int g_total;
FILE *g_file;
std::unique_ptr<LogFile> g_LogFile;

void dummyOutput(const char *msg, int len){
    g_total += len;
    if(g_file)
        fwrite(msg, 1, len, g_file);
    else if(g_LogFile)
        g_LogFile->append(msg, len);

}

void bench(const char *type){
    blackpanther::Logger::setOutput(dummyOutput);
    blackpanther::Timestamp start(blackpanther::Timestamp::now());
    g_total = 0;

    int n = 1000*1000;
    const bool kLongLog = false;
    std::string empty = " ";
    std::string longStr(3000, 'X');
    longStr += " ";
    for(int i = 0; i< n; ++i){
        LOG_INFO << "abcdefghijklmnopqrstuvwxyz" << " 0123456789" << (kLongLog? longStr : empty);
    }

    blackpanther::Timestamp end(blackpanther::Timestamp::now());

    double seconds = timeDifference(end, start);
    printf("%12s: %f seconds, %d bytes %10.2f msg/s, %.2f MiB/s\n",
    type, seconds, g_total, n/seconds, g_total/seconds/(1024*1024));
}

void logInThread(){
    LOG_INFO << "logInThread.";
}
int main(){
    getppid();

    blackpanther::ThreadPool pool("pool");
    pool.start(5);
    pool.run(logInThread);
    pool.run(logInThread);
    pool.run(logInThread);
    pool.run(logInThread);
    pool.run(logInThread);

    LOG_TRACE << "trace";
    LOG_DEBUG << "debug";
    LOG_INFO << "info";
    LOG_WARN << "warn";
    LOG_ERROR << "Error";

    LOG_INFO << sizeof(blackpanther::Logger);
    LOG_INFO << sizeof(blackpanther::LogStream);
    LOG_INFO << sizeof(blackpanther::fmt);
    LOG_INFO << sizeof(blackpanther::LogStream::Buffer);

    sleep(1);
    bench("nop");

    char buffer[64*1024];

    g_file = fopen("/dev/null", "w");
    setbuffer(g_file, buffer, sizeof(buffer));
    bench("/dev/null");
    fclose(g_file);
    return 0;
}