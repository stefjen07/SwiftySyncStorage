cmake_minimum_required(VERSION 3.19)
if (NOT TARGET SwiftySyncStorage)
    project(SwiftySyncStorage)

    set(CMAKE_CXX_STANDARD 14)

    add_subdirectory(SwiftySyncCommon)
    include_directories(SwiftySyncCommon)
    include_directories(SwiftySyncCommon/CodablePP)

    add_library(SwiftySyncStorage SwiftySyncStorage.cpp SwiftySyncStorage.h)
    target_link_libraries(SwiftySyncStorage SwiftySyncCommon Codable)

    add_executable(test_storage main.cpp)
    target_link_libraries(test_storage SwiftySyncStorage)
endif()