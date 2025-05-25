BISON_TARGET(M4Parser m4y.y ${CMAKE_CURRENT_BINARY_DIR}/m4y.c COMPILE_FLAGS -y)
add_executable(m4 m4.c ${BISON_M4Parser_OUTPUTS})
target_compile_options(m4 PRIVATE -Wall -Werror)
