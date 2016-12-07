//
// Created by mark on 11/21/16.
//

#include <blackpanther/base/ProcessInfo.h>

int main(){
    printf("pid=%d\n", blackpanther::ProcessInfo::pid());
    printf("uid=%d\n", blackpanther::ProcessInfo::uid());
    printf("euid=%d\n", blackpanther::ProcessInfo::euid());
    printf("hostname=%s\n", blackpanther::ProcessInfo::hostName().c_str());
    printf("opened files=%d\n", blackpanther::ProcessInfo::openedFiles());
    printf("threads=%d\n", blackpanther::ProcessInfo::numThreads());
    return 0;
}
