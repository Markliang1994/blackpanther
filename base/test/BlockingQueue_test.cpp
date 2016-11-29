//
// Created by mark on 16-11-29.
//

#include <blackpanther/base/BlockingQueue.h>
#include <blackpanther/base/Thread.h>
#include <blackpanther/base/CountDownLatch.h>

#include <boost/ptr_container/ptr_vector.hpp>

#include <string>

#include <stdio.h>
#include <unistd.h>

class Test{
public:
    Test(int numThreads):
            latch_(numThreads),
            threads_(numThreads){
        for(int i = 0; i < numThreads; ++i){
            char name[32];
            snprintf(name, sizeof(name), "work thread %d\n", i);
            threads_.push_back(new blackpanther::Thread(std::bind(&Test::threadFunc, this), std::string(name)));
        }
        for_each(threads_.begin(), threads_.end(), std::bind(&blackpanther::Thread::start, std::placeholders::_1));
    }

    void run(int times){
        printf("waiting for count down latch\n");
        latch_.wait();
        printf("all threads started.\n");

        for(int i = 0; i < times; ++i){
            char buf[32];
            snprintf(buf, sizeof(buf), "hello %d", i);
            queue_.put(buf);
            printf("tid=%d, put data = %s, size = %zd\n", blackpanther::CurrentThread::tid(), buf, queue_.size());
        }
    }

    void joinAll() {
        for (size_t i = 0; i < threads_.size(); ++i) {
            queue_.put("stop");
        };
        for_each(threads_.begin(), threads_.end(), std::bind(&blackpanther::Thread::join, std::placeholders::_1));
    }
private:

    void threadFunc(){
        printf("tid=%d %s started\n", blackpanther::CurrentThread::tid(), blackpanther::CurrentThread::name());

        latch_.countDown();
        bool running = true;
        while(running){
            std::string d(queue_.take());
            printf("tid=%d get data = %s, size = %zd\n", blackpanther::CurrentThread::tid(), d.c_str(), queue_.size());
            running = (d != "stop");
        }

        printf("tid=%d, %s stoped\n",
            blackpanther::CurrentThread::tid(), blackpanther::CurrentThread::name());
    }

    blackpanther::BlockingQueue<std::string> queue_;
    blackpanther::CountDownLatch latch_;
    boost::ptr_vector<blackpanther::Thread> threads_;
};

void testMove(){
    blackpanther::BlockingQueue<std::unique_ptr<int>> queue;
    queue.put(std::unique_ptr<int>(new int(42)));

    std::unique_ptr<int> x = queue.take();
    printf("took %d\n", *x);
    *x = 123;
    queue.put(std::move(x));
    std::unique_ptr<int> y = queue.take();
    printf("took %d\n", *y);
}

int main(int argc, char **argv){
    printf("pid=%d, tid=%d\n", ::getpid(), blackpanther::CurrentThread::tid());
    Test t(5);

    t.run(100);
    t.joinAll();

    //testMove();

    printf("numbers of created threads %d\n", blackpanther::Thread::numCreated());
    return 0;
}