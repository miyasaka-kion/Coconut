cmake_minimum_required(VERSION 3.14)
project(unit_test)

find_package(GTest REQUIRED)

add_executable(
    unit_test
    test_entity.cpp
)

target_include_directories(unit_test PRIVATE ${CMAKE_SOURCE_DIR}/Coconut/src)
target_link_libraries(unit_test PRIVATE 
    Coconut
    GTest::gtest_main
)

include(GoogleTest)
# message("Working directory = ${Coconut_BINARY_DIR}/${PROJECT_NAME}"
gtest_discover_tests(
    unit_test
)