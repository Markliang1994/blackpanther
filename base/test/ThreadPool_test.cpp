//
// Created by mark on 11/25/16.
//

#include <blackpanther/base/ThreadPool.h>
#include <blackpanther/base/CurrentThread.h>

#include <iostream>
using std::cout;
using std::endl;

void print(){
    printf("tid=%d\n", blackpanther::CurrentThread::tid());
}
void printString(const std::string &str){
    std::cout << str << std::endl;
}

void test(int maxSize){
    cout << "Test ThreadPool with max queue size = " << maxSize << std::endl;
    blackpanther::ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);

    pool.start(5);

    cout << "Adding" << std::endl;

    pool.run(print);

    pool.run(print);

    for(int i = 0; i < 100; i++){
        char buf[32];
        snprintf(buf, sizeof(buf), "task %d", i);
        pool.run(std::bind(printString, std::string(buf)));
    }
    cout << "Done" << std::endl;

    usleep(100*1000);
}

int main(int argc, char **argv){
    test(0);
    test(1);
    test(5);
    test(50);
    return 0;
}