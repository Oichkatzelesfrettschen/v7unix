cmake_minimum_required(VERSION 3.16)
project(v7unix C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_CLANG_TIDY clang-tidy)

# Export compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use ccache when available for faster builds
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

find_package(BISON)

if(BISON_FOUND)
    # Demonstrate the BISON_TARGET() helper by generating a small parser.
    BISON_TARGET(SampleGrammar
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/sample.y
        ${CMAKE_CURRENT_BINARY_DIR}/sample.c)
    add_custom_target(sample_parser DEPENDS ${BISON_SampleGrammar_OUTPUTS})
endif()

option(SPINLOCK_UNIPROCESSOR "Compile out spinlocks" OFF)
option(USE_TICKET_LOCK "Use fair ticket locks" OFF)
option(SPINLOCK_DEBUG "Instrument spinlocks" OFF)

if(SPINLOCK_UNIPROCESSOR)
    add_compile_definitions(SPINLOCK_UNIPROCESSOR)
endif()
if(USE_TICKET_LOCK)
    add_compile_definitions(USE_TICKET_LOCK)
endif()
if(SPINLOCK_DEBUG)
    add_compile_definitions(SPINLOCK_DEBUG)
endif()

add_subdirectory(v7/usr/src/cmd)
add_subdirectory(kernel)
