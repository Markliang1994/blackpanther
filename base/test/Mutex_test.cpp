//
// Created by mark on 11/20/16.
//

//#include <blackpanther/base/Thread.h>
#include <blackpanther/base/Mutex.h>

#include <iostream>
#include <stdlib.h>

#include <vector>

using namespace blackpanther;

MutexLock g_mutex;
int g_count;

void* tr1(void *){
    MutexLockGuard lock(g_mutex);
    g_mutex.assertLocked();
    g_count++;
    printf("The value is: %d\n", g_count);
}

int main(int argc, char **argv){
    std::vector<pthread_t*> vp;
    for(int i = 0; i < 1000; i++){
        vp.push_back(static_cast<pthread_t *>(new pthread_t));
    }

    for(int i = 0; i < 1000; i++) {
        pthread_create(vp[i], nullptr, tr1, nullptr);
    }

    for(int i = 0; i < 1000; i++){
        pthread_join(*vp[i], NULL);
    }

    for(int i = 0; i < 1000; i++){
        delete vp[i];
    }
    return 0;
}