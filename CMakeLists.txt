cmake_minimum_required(VERSION 3.5)
project(BLACKPANTHER)

INCLUDE_DIRECTORIES(/home/mark/Documents/workspace)
include_directories(/usr/include/boost)

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
        base/Logging.h base/LogStream.h base/Timestamp.h base/LogStream.cpp base/Logging.cpp base/Timestamp.cpp base/TimeZone.h base/TimeZone.cpp base/Date.h base/Date.cpp base/LogFile.cpp base/LogFile.h base/FileUtil.cpp base/FileUtil.h base/WeakCallback.h)

find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)
find_library(BOOSTPO_LIBRARY NAMES boost_program_options)

set(LIBRARY_OUTPUT_PATH  /home/mark/Documents/workspace/blackpanther)
add_library(blackpanther_base ${SOURCE_FILES})

add_subdirectory(base/tests)
add_subdirectory(net)
add_subdirectory(net/tests)