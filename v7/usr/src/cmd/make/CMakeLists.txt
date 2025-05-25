BISON_TARGET(MakeParser gram.y ${CMAKE_CURRENT_BINARY_DIR}/gram.c COMPILE_FLAGS -y)
add_executable(make
    ident.c doname.c misc.c files.c dosys.c main.c ${BISON_MakeParser_OUTPUTS}
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
