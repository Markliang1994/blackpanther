//
// Created by mark on 11/20/16.
//

#include <blackpanther/base/ProcessInfo.h>
#include <blackpanther/base/CurrentThread.h>

#include <algorithm>

#include <stdlib.h>
#include <assert.h>

#include <sys/resource.h>
#include <sys/times.h>
#include <pwd.h>
#include <dirent.h>

namespace blackpanther{
    namespace detail{
        __thread int t_numOpenedFiles = 0;
        int fdDirFilter(const struct dirent *d){
            if(::isdigit(d->d_name[0]))
                ++t_numOpenedFiles;
            return 0;
        }

        __thread std::vector<pid_t> *t_pids = nullptr;
        int taskDirFilter(const struct dirent *d){
            if(::isdigit(d->d_name[0])){
                t_pids->push_back(atoi(d->d_name));
            }
            return 0;
        }

        int scanDir(const char *dirpath, int (*filter)(const struct dirent *)){
            struct dirent **namelist = nullptr;
            int result = scandir(dirpath, &namelist, filter, alphasort);
            assert(namelist == nullptr);
            return  result;
        }

        int g_clockTicks = static_cast<int>(::sysconf(_SC_CLK_TCK));
        int g_pageSize = static_cast<int>(::sysconf(_SC_PAGE_SIZE));
    }
}

using namespace blackpanther;
using namespace detail;

pid_t  pid(){
    return ::getpid();
}

std::string pidString(){
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", pid());
    return buf;
}

uid_t ProcessInfo::uid() {
    return ::getuid();
}

std::string ProcessInfo::hostname(){
    char buf[256];
    if(::gethostname(buf, sizeof(buf)) == 0){
        buf[sizeof(buf)-1] = '\0';
        return buf;
    }
    else{
        return "unknowhost";
    }
}

std::string ProcessInfo::username() {
    struct passwd pwd;
    struct passwd *result = nullptr;

    char buf[8192];
    const char *name = "unknownuser";

    getpwuid_r(uid(), &pwd, buf, sizeof(buf), &result);
    if(result){
        name = pwd.pw_name;
    }
    return name;
}

uid_t ProcessInfo::euid(){
    return ::geteuid();
}

int ProcessInfo::clockTicksPerSecond() {
    return g_clockTicks;
}

int ProcessInfo::pageSize() {
    return g_pageSize;
}

bool isDebugBuild(){
#ifdef NDEBUG
    return false;
#else
    return true;
#endif
}

std::string ProcessInfo::procStatus() {
    std::string result;
    return result;
}

std::string ProcessInfo::procStat() {
    std::string result;
    return result;
}

std::string ProcessInfo::threadStat() {
    char buf[64];
    snprintf(buf, sizeof(buf), "/proc/slef/task/%d/stat", CurrentThread::tid());
    std::string result;
    return result;
}

std::string ProcessInfo::exePath() {
    std::string result;
    char buf[1024];
    ssize_t n = ::readlink("/proc/self/exe", buf, sizeof(buf));
    if(n > 0){
        result.assign(buf, n);
    }
    return result;
}

int ProcessInfo::openedFiles(){
    t_numOpenedFiles = 0;
    scanDir("/proc/self/fd", fdDirFilter);
    return t_numOpenedFiles;
}

int ProcessInfo::maxOpenFiles() {
    struct rlimit rl;
    if(::getrlimit(RLIMIT_NOFILE, &rl)){
        return openedFiles();
    }
    else{
        return static_cast<int>(rl.rlim_cur);
    }
}

ProcessInfo::CpuTime ProcessInfo::cpuTime() {
    ProcessInfo::CpuTime t;
    struct tms tms;
    if(::times(&tms)){
        const double hz = static_cast<double>(clockTicksPerSecond());
        t.userSeconds = static_cast<double>(tms.tms_utime) / hz;
        t.systemSeconds = static_cast<double>(tms.tms_stime) / hz;
    }
    return t;
}

int ProcessInfo::numThreads() {
    int result = 0;
    std::string status = procStatus();
    size_t pos = status.find("Threads:");
    if(pos != std::string::npos){
        result = atoi(status.c_str() + pos + 8);
    }
    return result;
}


std::vector<pid_t> ProcessInfo::threads(){
    std::vector<pid_t> result;
    t_pids = &result;
    scanDir("/proc/self/task", taskDirFilter);
    t_pids = NULL;
    std::sort(result.begin(), result.end());
    return result;

}