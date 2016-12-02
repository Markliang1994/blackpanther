cmake_minimum_required(VERSION 3.5)
project(BLACKPANTHER)

INCLUDE_DIRECTORIES(/home/mark/Documents/workspace)

set(CXX_FLAGS
        -g
        -O2
        -Wall
        -Wconversion
        -Wno-unused-parameter
        -Wpointer-arith
        -march=native
        -Wwrite-strings
        -Wextra
        -Werror
        -Wshadow
        -pthread
        -rdynamic
        -std=c++11)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(SOURCE_FILES base/Mutex.h base/Condition.h base/condition.cpp base/CurrentThread.h base/Singleton.h base/ProcessInfo.h base/Thread.h base/Thread.cpp base/Atomic.h base/ProcessInfo.cpp base/Exception.h base/Exception.cpp base/copyable.h base/noncopyable.h base/ThreadPool.h base/ThreadPool.cpp base/BlockingQueue.h base/CountDownLatch.cpp base/CountDownLatch.h
        base/Logging.h base/LogStream.h base/Timestamp.h base/LogStream.cpp base/Logging.cpp base/Timestamp.cpp base/TimeZone.h base/TimeZone.cpp)
add_library(blackpanther_base ${SOURCE_FILES})

#add_executable(blackpanther ${SOURCE_FILES})
add_subdirectory(base/test)
