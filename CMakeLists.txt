cmake_minimum_required(VERSION 3.16)
project(v7unix C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_CLANG_TIDY clang-tidy)

include(FindBISON)

add_subdirectory(v7/usr/src/cmd)
