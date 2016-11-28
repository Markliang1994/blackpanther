//
// Created by mark on 11/21/16.
//

#include <blackpanther/base/ProcessInfo.h>

#include <iostream>

int main(int argc, char **argv){
    std::string name;
    name = blackpanther::ProcessInfo::username();
    std::cout << name << std::endl;
    return 0;
}
