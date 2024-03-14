project(LevelEditor)

aux_source_directory(src SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Coconut/src)

target_link_libraries(${PROJECT_NAME} PRIVATE Coconut)

# post build commands
file(COPY "${CMAKE_SOURCE_DIR}/Coconut/assets" DESTINATION "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")