cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(box2dtest)

find_package(box2d REQUIRED)
find_package(sdl2 REQUIRED)
find_package(sdl2_image REQUIRED)


message("box2d_FOUND = ${box2d_FOUND}")
message("sdl2_FOUND = ${sdl2_FOUND}")
message("sdl2_image_FOUND = ${sdl2_image_FOUND}")
message("box2d_VERSION = ${box2d_VERSION}")
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE box2d::box2d)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)