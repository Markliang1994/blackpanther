//
// Created by mark on 16-11-29.
//
#include <blackpanther/base/Exception.h>
#include <stdio.h>

class Bar{
public:
    void test(){
        throw blackpanther::Exception("oops");
    }
};

void foo(){
    Bar b;
    b.test();
}

int main(){
    try{
        foo();
    }
    catch(const blackpanther::Exception &ex){
        printf("reason: %s\n", ex.what());
        printf("stack trace: %s\n", ex.stackTrace());
    }
    return 0;
}

