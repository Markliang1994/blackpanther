//
// Created by mark on 11/19/16.
//

#ifndef BLACKPANTHER_BASE_PROCESSINFO_H
#define BLACKPANTHER_BASE_PROCESSINFO_H


#include <unistd.h>
#include <sys/types.h>

#include <vector>
#include <string>

namespace blackpanther{
    namespace ProcessInfo{
        pid_t pid();
        std::string pidString();
        uid_t uid();
        std::string username();
        uid_t euid();
        int clockTicksPerSecond();
        int pageSize();
        bool isDebugBuild();

        std::string hostName();
        std::string proceName();

        std::string procStatus();
        std::string procStat();

        std::string threadStat();

        std::string exePath();

        int openedFiles();
        int maxOpenFiles();

        struct CpuTime{
            double userSeconds;
            double systemSeconds;

            CpuTime(): userSeconds(0.0), systemSeconds(0.0) {  }
        };
        CpuTime cpuTime();

        int numThreads();
        std::vector<pid_t> threads();
    }
}
#endif //BLACKPANTHER_PROCESSINFO_H
