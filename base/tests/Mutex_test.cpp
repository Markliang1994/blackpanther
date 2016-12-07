//
// Created by mark on 11/20/16.
//

#include <blackpanther/base/Thread.h>
#include <blackpanther/base/Condition.h>
#include <blackpanther/base/Mutex.h>

#include <iostream>
#include <stdlib.h>

#include <queue>

using std::queue;
using namespace blackpanther;

MutexLock mutex_;
Condition notFull(mutex_);
Condition notEmpty(mutex_);

queue<int> buf;
const int MAXSIZE = 1000;
void producer(void){
    while(true){
        mutex_.lock();
        if(buf.size() >= MAXSIZE)
            notFull.wait();
        buf.push(1);
        printf("add element to buf. now buf size is: %lu\n", buf.size());
        if(!buf.empty())
            notEmpty.notify();
        mutex_.unlock();
    }
}

void consumer2(void){
    while(true){
        mutex_.lock();
        while(buf.size() <= 50)
            notEmpty.wait();
        buf.pop();
        printf("buf size is :%lu\t spurious weakup happending!\n",buf.size());
        printf("consume element from buf. now buf size is: %lu\n", buf.size());
        if(buf.size() == MAXSIZE -1)
            notFull.notify();
        mutex_.unlock();
    }
}
void consumer1(void){
    while(true){
        mutex_.lock();
        if(buf.empty())
            notEmpty.wait();
        buf.pop();
        printf("consume element from buf. now buf size is: %lu\n", buf.size());
        if(buf.size() == MAXSIZE -1)
            notFull.notify();
        mutex_.unlock();
    }
}

int main(void){
    Thread t1(producer);
    Thread t2(consumer1);
    Thread t3(consumer2);

    t1.start();
    t2.start();

    t3.start();

    t1.join();
    t2.join();
    t3.join();
    return 0;
}