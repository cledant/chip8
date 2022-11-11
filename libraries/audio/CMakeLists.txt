cmake_minimum_required(VERSION 3.17)
project(lib_audio)

add_library(audio STATIC
        private/audio.c)
target_include_directories(audio
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
set_target_properties(audio PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS YES)
add_dependencies(audio SDL2-static)
target_link_libraries(audio SDL2-static)
target_compile_options(audio PRIVATE -Wall -Wextra -Werror)