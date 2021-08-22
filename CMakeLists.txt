cmake_minimum_required(VERSION 3.0)
if (NOT TARGET SwiftySyncStorage)
    project(SwiftySyncStorage)

    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_C_STANDARD 11)

    add_subdirectory(SwiftySyncCommon)

    include_directories(include)

    add_library(SwiftySyncStorage src/SwiftySyncStorage.cpp include/SwiftySyncStorage.hpp)
    target_link_libraries(SwiftySyncStorage SwiftySyncCommon Codable)
    target_include_directories(SwiftySyncStorage PUBLIC SwiftySyncCommon/include PUBLIC SwiftySyncCommon/CodablePP/include)

    add_library(c_api_storage_helper src/c_api_storage_helper.cpp include/c_api_storage_helper.hpp)
    target_link_libraries(c_api_storage_helper SwiftySyncStorage)

    add_library(c_api_storage src/c_api_storage.cpp include/c_api_storage.h)
    target_link_libraries(c_api_storage SwiftySyncStorage c_api_storage_helper)

    #add_executable(test_storage test/test.cpp)
    #target_link_libraries(test_storage SwiftySyncStorage)

    #add_executable(test_c_api_storage test/test_c_api_storage.c)
    #target_link_libraries(test_c_api_storage c_api_storage)
endif()