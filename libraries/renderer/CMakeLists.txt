cmake_minimum_required(VERSION 3.17)
project(lib_renderer)

add_library(renderer STATIC
        private/renderer.c)
target_include_directories(renderer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
set_target_properties(renderer PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS YES)
add_dependencies(renderer SDL2-static)
target_link_libraries(renderer SDL2-static)
target_compile_options(renderer PRIVATE -Wall -Wextra -Werror)