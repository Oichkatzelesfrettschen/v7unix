BISON_TARGET(NeqnParser e.y ${CMAKE_CURRENT_BINARY_DIR}/e.c
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/e.def
             COMPILE_FLAGS "-y -d")
add_executable(neqn
    diacrit.c eqnbox.c funny.c io.c lex.c move.c pile.c sqrt.c
    font.c glob.c integral.c lookup.c mark.c matrix.c over.c paren.c
    shift.c size.c text.c fromto.c ${BISON_NeqnParser_OUTPUTS}
)
target_compile_options(neqn PRIVATE -Wall -Werror)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DNEQN)
