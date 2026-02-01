add_executable(run_tests
    src/NodeTests.cpp
)
target_include_directories(run_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(run_tests
    eSGraph
    gtest
    gtest_main
)
add_test(NAME NodeTests COMMAND run_tests)
