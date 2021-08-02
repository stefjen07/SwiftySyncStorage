cmake_minimum_required(VERSION 3.19)
if (NOT TARGET SwiftySyncStorage)
    project(SwiftySyncStorage)
    project(c-api-storage)

    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_C_STANDARD 11)

    add_subdirectory(SwiftySyncCommon)

    include_directories(/)

    add_library(SwiftySyncStorage SwiftySyncStorage.cpp SwiftySyncStorage.h)
    target_link_libraries(SwiftySyncStorage SwiftySyncCommon Codable)
    target_include_directories(SwiftySyncStorage PUBLIC SwiftySyncCommon PUBLIC SwiftySyncCommon/CodablePP)

    add_library(c_api_storage c_api_storage.cpp c_api_storage.h)
    target_link_libraries(c_api_storage SwiftySyncStorage)

    add_executable(test_storage main.cpp)
    target_link_libraries(test_storage SwiftySyncStorage)

    add_executable(test_c_api_storage test_c_api_storage.c)
    target_link_libraries(test_c_api_storage c_api_storage)
endif()