project(LevelEditor)

aux_source_directory(src SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE Coconut)

