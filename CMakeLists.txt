cmake_minimum_required(VERSION 2.8)

project(commands-logging)

set(SOURCE main.cpp)
set(HEADER)

set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(commands-logging ${SOURCE})