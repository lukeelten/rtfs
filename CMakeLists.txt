cmake_minimum_required(VERSION 3.8)
project(rtfs)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp)
include_directories(include)

add_executable(rtfs ${SOURCE_FILES})
