cmake_minimum_required(VERSION 3.16)
project(stlib_tests)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ext/Unity/src)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../stlib)

file(GLOB_RECURSE TEST_SOURCES "*.c")

add_executable(run_tests
    ${TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/Unity/src/unity.c
)

target_link_libraries(run_tests stlib)

enable_testing()
add_test(NAME stlib_tests COMMAND run_tests)
