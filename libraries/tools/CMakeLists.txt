cmake_minimum_required(VERSION 3.17)
project(lib_tools)

add_library(tools STATIC
        private/tools.c)
target_include_directories(tools
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
set_target_properties(tools PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS YES)
target_compile_options(tools PRIVATE -Wall -Wextra -Werror)