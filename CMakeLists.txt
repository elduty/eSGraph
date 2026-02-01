cmake_minimum_required(VERSION 3.14)
project(eSGraph VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main library
add_library(eSGraph STATIC src/Node.cpp)
target_include_directories(eSGraph PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
)

# Tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(external/googletest)
    add_subdirectory(tests)
endif()
