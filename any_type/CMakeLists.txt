CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(MAIN)

ADD_DEFINITIONS(-std=c++11)

ADD_DEFINITIONS(-g -ggdb)

ADD_DEFINITIONS(-Wall)

SET(CMAKE_BUILD_TYPE "Release")

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

FIND_PACKAGE (Threads)

ADD_EXECUTABLE(main ${DIR_SRCS})

TARGET_LINK_LIBRARIES (main ${CMAKE_THREAD_LIBS_INIT})