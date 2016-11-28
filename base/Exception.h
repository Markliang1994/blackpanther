//
// Created by mark on 11/21/16.
//

#ifndef BLACKPANTHER_BASE_EXCEPTION_H
#define BLACKPANTHER_BASE_EXCEPTION_H

#include <exception>
#include <string>

namespace blackpanther{
    class Exception : std::exception{
    public:
        explicit Exception(const char *what);
        explicit Exception(const std::string &what);
        virtual const char *what() const throw();
        const char *stackTrace() const throw();

    private:
        void fillStackTrace();

        std::string message_;
        std::string stack_;
    };
}

#endif //BLACKPANTHER_EXCEPTION_H
