project(SDL_test)

message("${PROJECT_NAME}: (Debug: CMake before)Source List: ${SRC_LIST}")

aux_source_directory(src SRC_LIST)
message("${PROJECT_NAME}: (Debug: CMake aux_source_directory after)Source List: ${SRC_LIST}")

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} CoconutEngine)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${SRC_LIST}
)

# yet this has no practical effect
target_link_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${SRC_LIST}
)
