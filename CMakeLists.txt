cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(EXPORT_COMPILE_COMMANDS ON)

set(APPLICATION_NAME "SDL_Test")

project(SDL_Game)
# Define output directory names based on build configuration
# set(OUTPUT_DIR_NAME "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

add_subdirectory(CoconutEngine)
add_subdirectory(${APPLICATION_NAME})

file(COPY "${CMAKE_SOURCE_DIR}/${APPLICATION_NAME}/assets" DESTINATION "${CMAKE_BINARY_DIR}/${APPLICATION_NAME}")
