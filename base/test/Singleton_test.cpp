//
// Created by mark on 16-11-29.
//

#include <blackpanther/base/Singleton.h>
#include <blackpanther/base/CurrentThread.h>
#include <blackpanther/base/Thread.h>

#include <blackpanther/base/noncopyable.h>

class Test : blackpanther::noncopyable{
public:
    Test(){
        printf("tid = %d, constructing %p\n", blackpanther::CurrentThread::tid(), this);
    }

    ~Test(){
        printf("tid = %d, destructing %p %s\n", blackpanther::CurrentThread::tid(), this, name_.c_str());
    }
    const std::string &name() const { return name_; }
    void setName(const std::string &name) { name_ = name; }
private:
    std::string name_;
};

class TestNoDestroy : blackpanther::noncopyable{
public:
    void no_destroy();

    TestNoDestroy(){
        printf("tid = %d, constructing TestNoDestroy %p\n", blackpanther::CurrentThread::tid(), this);
    }

    ~TestNoDestroy(){
        printf("tid = %d, destructing TestNoDestroy %p\n", blackpanther::CurrentThread::tid(), this);
    }
};

void threadFunc(){
    printf("tid = %d, %p name = %s\n", blackpanther::CurrentThread::tid(), &blackpanther::Singleton<Test>::instance(),
        blackpanther::Singleton<Test>::instance().name().c_str());

    blackpanther::Singleton<Test>::instance().setName("only one, changed.");
}

int main(int argc, char **argv){
    blackpanther::Singleton<Test>::instance().setName("only one.");
    blackpanther::Thread t1(threadFunc);
    t1.start();
    t1.join();

    printf("tid = %d, %p, name = %s\n", blackpanther::CurrentThread::tid(), &blackpanther::Singleton<Test>::instance(),
           blackpanther::Singleton<Test>::instance().name().c_str());
    blackpanther::Singleton<TestNoDestroy>::instance();
    printf("with valgrind, you should see %zd-byte memory leak.\n", sizeof(TestNoDestroy));

    return 0;
}