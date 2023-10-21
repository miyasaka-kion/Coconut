cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(box2dtest)

aux_source_directory(src SRC_LIST)

find_package(box2d REQUIRED)
find_package(sdl2 REQUIRED)
find_package(sdl2_image REQUIRED)
find_package(spdlog REQUIRED)

message("box2d_FOUND = ${box2d_FOUND}")


add_executable(${PROJECT_NAME} ${SRC_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE box2d::box2d)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)
# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

add_subdirectory("vendor/imgui")
target_link_libraries(${PROJECT_NAME} PRIVATE imgui_sdl2_sdlrenderer2)

# post build commands
file(COPY "${CMAKE_SOURCE_DIR}/source/assets" DESTINATION "${CMAKE_BINARY_DIR}")