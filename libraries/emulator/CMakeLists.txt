cmake_minimum_required(VERSION 3.17)
project(lib_emulator)

add_library(emulator STATIC
        private/emulator.c
        private/emu_chip8.c
        private/emu_superchip8.c
        private/emu_chip8_cosmac_vip.c
        private/emu_quirks.c
        private/emu_inst.c)
target_include_directories(emulator
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
set_target_properties(emulator PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS YES)
target_compile_options(emulator PRIVATE -Wall -Wextra -Werror)