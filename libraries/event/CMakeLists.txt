cmake_minimum_required(VERSION 3.17)
project(lib_event_buffer)

add_library(event_buffer STATIC
        private/event_buffer.c)
target_include_directories(event_buffer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
set_target_properties(event_buffer PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO)
target_compile_options(event_buffer PRIVATE -Wall -Wextra -Werror)