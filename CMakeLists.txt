cmake_minimum_required(VERSION 3.19)
project(SwiftySyncStorage)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(SwiftySyncCommon)
include_directories(SwiftySyncCommon)
include_directories(SwiftySyncCommon/CodablePP)

add_library(SwiftySyncStorage SwiftySyncStorage.h SwiftySyncStorage.cpp)