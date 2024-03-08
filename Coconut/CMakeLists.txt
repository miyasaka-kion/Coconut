cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Coconut)

aux_source_directory("src" SRC_LIST)
aux_source_directory("src/ECS"  SRC_LIST)
aux_source_directory("src/Render" SRC_LIST)
aux_source_directory("src/Core" SRC_LIST)
aux_source_directory("src/Event" SRC_LIST)
aux_source_directory("src/util" SRC_LIST)
message("SRC_LIST = ${SRC_LIST}")


find_package(box2d REQUIRED)
find_package(sdl2 REQUIRED)
find_package(sdl2_image REQUIRED)
find_package(spdlog REQUIRED)
# find_package(entt REQUIRED)


# add_executable(${PROJECT_NAME} ${SRC_LIST})
add_library(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PUBLIC box2d::box2d)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image::SDL2_image)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog_header_only)
# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2main)
endif()

message("imgui source dir: ${CMAKE_SOURCE_DIR}/vendor/imgui}")
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/imgui" build)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui_sdl2_sdlrenderer2)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/vendor/entt/include")

message("entt include dir: ${CMAKE_SOURCE_DIR}/vendor/entt/include")

target_compile_definitions(${PROJECT_NAME} PRIVATE COCONUT_ASSET_PATH="${CMAKE_SOURCE_DIR}/assets")
