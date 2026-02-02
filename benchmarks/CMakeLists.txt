# Benchmarks CMakeLists.txt

add_executable(run_benchmarks
    src/BenchmarkMain.cpp
    src/HierarchyBuilders.cpp
    src/NodeBenchmarks.cpp
)

target_include_directories(run_benchmarks PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(run_benchmarks PRIVATE eSGraph)

# Ensure optimizations for accurate benchmarking in Release mode
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(run_benchmarks PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-O3 -DNDEBUG>
        $<$<CXX_COMPILER_ID:MSVC>:/O2 /DNDEBUG>
    )
endif()
