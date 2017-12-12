cmake_minimum_required(VERSION 3.8)
project(rtfs)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp src/rtfs/formatter.cpp src/rtfs/inode.cpp src/rtfs/superblock.cpp src/rtfs/instance.cpp src/rtfs/internal.cpp src/rtfs/file.cpp include/rtfs/file.h src/rtfs/inode_address.cpp include/rtfs/folder.h src/rtfs/folder.cpp)
include_directories(include)

find_package(PkgConfig)
pkg_check_modules (FUSE REQUIRED fuse3)

add_definitions(${FUSE_CFLAGS} ${FUSE_CFLAGS_OTHER})
include_directories(${FUSE_INCLUDE_DIRS})
link_directories(${FUSE_LIBRARY_DIRS})

add_executable(rtfs ${SOURCE_FILES})

target_link_libraries(rtfs ${FUSE_LIBRARIES})
