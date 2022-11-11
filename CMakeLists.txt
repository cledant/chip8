cmake_minimum_required(VERSION 3.17)
project(chip8_emu)

#Detecting + setting global LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT lto_output)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND lto_supported)
    message("Compiling with LTO enabled")
    cmake_policy(SET CMP0069 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else ()
    message("LTO not supported")
endif ()

#Runtime resources
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#Setting global C/CXX compile options
set(OPTIMIZATION_TYPE NATIVE CACHE STRING "")

if (${OPTIMIZATION_TYPE} STREQUAL NATIVE)
    message("Build binary for current computer arch")
    set(CMAKE_CXX_FLAGS "-march=native -pipe")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
elseif (${OPTIMIZATION_TYPE} STREQUAL AVX2)
    message("Build binary with AVX2 profile")
    set(CMAKE_CXX_FLAGS "-march=core2 -mtune=haswell \
    -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 \
    -mavx -mavx2 -mbmi -mbmi2 -mf16c -mfma -mfsgsbase \
    -mlzcnt -mmovbe -mpclmul -mpopcnt -maes \
    -mrdrnd  -pipe")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
elseif (${OPTIMIZATION_TYPE} STREQUAL SSE4)
    message("Build binary with SSE4 profile")
    set(CMAKE_CXX_FLAGS "-march=core2 -mtune=nehalem \
    -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 \
    -mpopcnt -pipe")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
else ()
    message("No arch optimization selected")
    set(CMAKE_CXX_FLAGS "-pipe")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
endif ()

#Setup SDL2
option(BUILD_SHARED_LIBS "" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2 ${CMAKE_BINARY_DIR}/SDL2/build)

#Project libs
add_subdirectory(libraries/emulator)
add_subdirectory(libraries/renderer)
add_subdirectory(libraries/engine)
add_subdirectory(libraries/input)
add_subdirectory(libraries/event)
add_subdirectory(libraries/tools)
add_subdirectory(libraries/audio)

#Main binary
add_executable(chip8_emu
        ${CMAKE_CURRENT_SOURCE_DIR}/binary/private/chip8_emu.c
        ${CMAKE_CURRENT_SOURCE_DIR}/binary/private/args_parsing.c)
target_include_directories(chip8_emu
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/binary/private)
target_link_libraries(chip8_emu emulator renderer engine input audio)
add_dependencies(chip8_emu emulator renderer engine input audio)
set_target_properties(chip8_emu PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS YES)
target_compile_options(chip8_emu PRIVATE -Wall -Wextra -Werror)
