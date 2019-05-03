cmake_minimum_required(VERSION 3.8)
project(AIcode)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp SearchTree.h SearchTree.cpp)
add_executable(AIcode ${SOURCE_FILES})