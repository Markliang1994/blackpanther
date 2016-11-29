cmake_minimum_required(VERSION 3.5)
project(BLACKPANTHER)

INCLUDE_DIRECTORIES(/home/mark/Documents/workspace)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")

set(SOURCE_FILES base/Mutex.h base/Condition.h base/condition.cpp base/CurrentThread.h base/Singleton.h base/ProcessInfo.h base/Thread.h base/Thread.cpp base/Atomic.h base/ProcessInfo.cpp base/Exception.h base/Exception.cpp base/copyable.h base/noncopyable.h base/ThreadPool.h base/ThreadPool.cpp base/BlockingQueue.h base/CountDownLatch.cpp base/CountDownLatch.h)
add_library(blackpanther_base ${SOURCE_FILES})

#add_executable(blackpanther ${SOURCE_FILES})
add_subdirectory(base/test)
