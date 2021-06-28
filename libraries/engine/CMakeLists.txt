cmake_minimum_required(VERSION 3.17)
project(lib_engine)

add_library(engine STATIC
        private/engine.c)
target_include_directories(engine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
set_target_properties(engine PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO)
target_link_libraries(engine input event_buffer emulator renderer tools audio)
add_dependencies(engine input event_buffer emulator renderer tools audio)
target_compile_options(engine PRIVATE -Wall -Wextra -Werror)