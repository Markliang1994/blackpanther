//
// Created by mark on 16-12-6.
//
#include <blackpanther/base/Logging.h>
#include <blackpanther/base/LogFile.h>

std::unique_ptr<blackpanther::LogFile> g_logFile;

void outputFunc(const char *msg, int len){
    g_logFile->append(msg, len);
}

void flushFunc(){
    g_logFile->flush();
}

int main(int argc, char **argv){
    char name[256];
    strncpy(name, argv[0], 256);
    printf("%s\n", argv[0]);
    g_logFile.reset(new blackpanther::LogFile(::basename(name), 200*1000));
    printf("%s\t%s\n", ::basename(name), name);
    blackpanther::Logger::setOutput(outputFunc);
    blackpanther::Logger::setFlush(flushFunc);

    std::string line="0123456789 abcdfghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i = 0; i < 10000; ++i){
        LOG_INFO << line << i;
        usleep(1000);
    }
    return 0;
}
