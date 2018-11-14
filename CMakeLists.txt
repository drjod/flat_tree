cmake_minimum_required(VERSION 2.6)
project(FlatTree)
set(FlatTree__VERSION_MAJOR 1)
set(FlatTree_VERSION_MINOR 0)


include_directories(                                                                                          
        "${PROJECT_BINARY_DIR}"
) 

set(CMAKE_CXX_FLAGS "-Wall -pthread")
set(CMAKE_CXX_STANDARD 11)

add_subdirectory("${PROJECT_SOURCE_DIR}/ext/gtest-1.8.0")

enable_testing()

include_directories(
		"${PROJECT_SOURCE_DIR}"
		"${gtest_SOURCE_DIR}"
		"${gtest_SOURCE_DIR}/include"
)

add_executable(run_tests gtest/allTests.cpp)
target_link_libraries(run_tests gtest gtest_main) 
