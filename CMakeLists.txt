cmake_minimum_required(VERSION 3.5)
project(blackpanther C CXX)

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

find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)
find_library(BOOSTPO_LIBRARY NAMES boost_program_options)

set(LIBRARY_OUTPUT_PATH  /home/mark/Documents/workspace/blackpanther)

add_subdirectory(base)
add_subdirectory(base/tests)
add_subdirectory(net)
add_subdirectory(net/tests)