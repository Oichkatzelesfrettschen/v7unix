cmake_minimum_required(VERSION 3.16)
project(v7unix C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_CLANG_TIDY clang-tidy)

# Export compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use ccache when available for faster builds
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

include(FindBISON)

add_subdirectory(v7/usr/src/cmd)
