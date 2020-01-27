cmake_minimum_required(VERSION 3.15)
project(Snake_Cpp)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} sources)
add_executable(${PROJECT_NAME} ${sources})