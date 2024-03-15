cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(EXPORT_COMPILE_COMMANDS ON)

set(APPLICATION_NAME "Coconut_test")

project(SDL_Game)
# Define output directory names based on build configuration
# set(OUTPUT_DIR_NAME "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

message("CMAKE_ASSET_DIR = ${CMAKE_SOURCE_DIR}/assets")
message("CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

include(CTest)

add_subdirectory(Coconut)
add_subdirectory(LevelEditor)
# add_subdirectory(GameDemo)

