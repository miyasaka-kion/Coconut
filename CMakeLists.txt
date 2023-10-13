cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(box2dtest)

aux_source_directory(src SRC_LIST)

message("SRC_LIST: ${SRC_LIST}")

find_package(box2d CONFIG REQUIRED)
find_package(sdl2 REQUIRED)
find_package(sdl2_image REQUIRED)

message("box2d_FOUND = ${box2d_FOUND}")

if(WIN32)
add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST})
message("add exec WIN32")
else()
add_executable(${PROJECT_NAME} ${SRC_LIST})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE box2d::box2d)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)
