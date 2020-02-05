cmake_minimum_required(VERSION 3.15)
project(snake_cpp)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} sources)
add_executable(${PROJECT_NAME} ${sources})