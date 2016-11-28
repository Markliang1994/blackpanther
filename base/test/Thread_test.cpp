//
// Created by mark on 11/21/16.
//

#include <blackpanther/base/CurrentThread.h>
#include <blackpanther/base/Thread.h>
#include <iostream>

using namespace blackpanther;


void threadFunc(){
    std::cout << "threadFunc: luke skywalker is a jedi." << std::endl;
}

void threadFunc2(int x){
    printf("threadFunc2: tid=%d\tx=%d\n", blackpanther::CurrentThread::tid(), x);
}

void threadFunc3(){
    printf("threadFunc3: tid=%d\n", blackpanther::CurrentThread::tid());
}

class Foo{
public:
    explicit Foo(double x):
            x_(x){

    }
    void memberFunc(){
        printf("tid=%d, Foo::x_=%f\n", blackpanther::CurrentThread::tid(), x_);
    }

    void memberFunc2(const std::string &text){
        printf("tid=%d, Foo::x_=%f, text=%s\n", blackpanther::CurrentThread::tid(), x_, text.c_str());
    }

private:
    double  x_;
};

int main(int argc, char **argv){
    Thread t1(threadFunc);

    t1.start();
    t1.join();

    Thread t2(std::bind(threadFunc2, 22), "thread for free function with args.");
    t2.start();
    t2.join();

    Foo foo(22.00);
    Thread t3(std::bind(&Foo::memberFunc, &foo), "thread for member function without arguments.");
    t3.start();
    t3.join();

    Thread t4(std::bind(&Foo::memberFunc2, std::ref(foo), std::string("Shaocong Liang")));
    t4.start();
    t4.join();

    {
        Thread t5(threadFunc3);
        t5.start();
        //local variable destruct eariler than thread creation.
    }
    return 0;
}