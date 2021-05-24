cmake_minimum_required(VERSION 3.17)
project(lib_input)

add_library(input STATIC
        private/input.c)
target_include_directories(input
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
set_target_properties(input PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO)
add_dependencies(input event_buffer SDL2-static)
target_link_libraries(input event_buffer SDL2-static)
target_compile_options(input PRIVATE -Wall -Wextra -Werror)