 cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

# Define output directory names based on build configuration
# set(OUTPUT_DIR_NAME "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

add_subdirectory(CoconutEngine)
add_subdirectory(SDL_Test)