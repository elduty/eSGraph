cmake_minimum_required(VERSION 3.14)
project(eSGraph VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

# Main library
add_library(eSGraph STATIC src/Node.cpp)
target_include_directories(eSGraph PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/glm>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/glm>
)

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(eSGraph PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(eSGraph PRIVATE /W4)
endif()

# AddressSanitizer
if(ENABLE_ASAN)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
endif()

# Code coverage
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif()
endif()

# Tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    # Force Google Test to use shared runtime on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(external/googletest)
    add_subdirectory(tests)
endif()

# Benchmarks
option(BUILD_BENCHMARKS "Build performance benchmarks" OFF)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# Install targets
include(GNUInstallDirs)

install(TARGETS eSGraph
    EXPORT eSGraphTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES include/Node.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/eSGraph
)

install(DIRECTORY external/glm/glm
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT eSGraphTargets
    FILE eSGraphTargets.cmake
    NAMESPACE eSGraph::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eSGraph
)

# Package configuration
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/eSGraphConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/eSGraphConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eSGraph
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/eSGraphConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/eSGraphConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/eSGraphConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eSGraph
)
