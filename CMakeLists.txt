cmake_minimum_required(VERSION 3.19)
if (NOT TARGET SwiftySyncStorage)
    project(SwiftySyncStorage)

    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_C_STANDARD 11)

    add_subdirectory(SwiftySyncCommon)

    include_directories(/)
    include_directories(SwiftySyncCommon)
    include_directories(SwiftySyncCommon/CodablePP)

    add_library(c_api_storage c_api_storage.cpp c_api_storage.h)

    #add_executable(test_storage main.cpp)

    #add_executable(test_c_api_storage test_c_api_storage.c)
    #target_link_libraries(test_c_api_storage c_api_storage)
endif()