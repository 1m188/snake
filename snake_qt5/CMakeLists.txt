cmake_minimum_required(VERSION 3.15)
project(snake_qt5 LANGUAGES CXX)

find_package(Qt5 REQUIRED Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} sources)
add_executable(${PROJECT_NAME} ${sources})

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:RELEASE>:NDEBUG>)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Snake_Qt.qrc)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

if(MSVC_IDE)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/subsystem:windows /entry:mainCRTStartup")
endif()