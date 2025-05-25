file(GLOB SYS_SRC "${CMAKE_SOURCE_DIR}/v7/usr/sys/sys/*.c")
file(GLOB DEV_SRC "${CMAKE_SOURCE_DIR}/v7/usr/sys/dev/*.c")
set(KERNEL_SOURCES ${SYS_SRC} ${DEV_SRC}
    ${CMAKE_SOURCE_DIR}/v7/usr/sys/conf/c.c
    ${CMAKE_SOURCE_DIR}/src/kernel/exo_mailbox.c
    ${CMAKE_SOURCE_DIR}/src/kernel/exo_ipc_queue.c)
add_executable(kernel ${KERNEL_SOURCES})
target_include_directories(kernel PRIVATE
    ${CMAKE_SOURCE_DIR}/v7/usr/sys/h
    ${CMAKE_SOURCE_DIR}/src/kernel)
target_compile_options(kernel PRIVATE -std=gnu90 -Wno-return-type -Wno-deprecated-non-prototype -fno-builtin -fno-builtin-malloc)
