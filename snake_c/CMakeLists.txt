cmake_minimum_required(VERSION 3.15)

if(NOT WIN32)
    message(FATAL_ERROR "Platform is not windows, and this project can just run in windows.")
endif()

project(snake_c LANGUAGES C)

file(GLOB_RECURSE sources src/*.c)
add_executable(${PROJECT_NAME} ${sources})

# recurse include path
function(target_include_directories_recurse target attr root)
    if(IS_DIRECTORY ${root})
        target_include_directories(${target} ${attr} ${root})
    endif()
    file(GLOB sub_srcs RELATIVE ${root} ${root}/*)
    foreach(sub ${sub_srcs})
        if(IS_DIRECTORY ${root}/${sub})
            target_include_directories_recurse(${target} ${attr} ${root}/${sub})
        endif()
    endforeach()
endfunction()
target_include_directories_recurse(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()