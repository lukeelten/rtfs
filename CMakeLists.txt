cmake_minimum_required(VERSION 3.8)
project(rtfs)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp include/rtfs.h)
include_directories(include)

find_package(PkgConfig)
pkg_check_modules (FUSE REQUIRED fuse3)

add_definitions(${FUSE_CFLAGS} ${FUSE_CFLAGS_OTHER})
include_directories(${FUSE_INCLUDE_DIRS})
link_directories(${FUSE_LIBRARY_DIRS})

add_executable(rtfs ${SOURCE_FILES})

target_link_libraries(rtfs ${FUSE_LIBRARIES})
