//
// Created by mark on 11/21/16.
//

#include <blackpanther/base/Exception.h>

using namespace blackpanther;

Exception::Exception(const char *msg):
    message_(msg){

}

Exception::Exception(const std::string &msg):
    message_(msg){

}

const char *Exception::what() const throw(){
    return message_.c_str();
}

const char *Exception::stackTrace() const throw(){
    return stack_.c_str();
}

void Exception::fillStackTrace() {

}