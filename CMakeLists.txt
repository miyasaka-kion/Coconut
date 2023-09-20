cmake_minimum_required(VERSION 3.15)

project(CoconutEngine)

set(CMAKE_CXX_STANDARD 17)

# Define output directory names based on build configuration
set(OUTPUT_DIR_NAME "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")




find_package(spdlog REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable()
target_link_libraries(spdtest PRIVATE SDL2::SDL2)
target_link_libraries(spdtest PRIVATE spdlog::spdlog_header_only)

