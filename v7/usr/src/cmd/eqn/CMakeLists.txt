BISON_TARGET(EqnParser e.y ${CMAKE_CURRENT_BINARY_DIR}/e.c
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/e.def
             COMPILE_FLAGS -y -d)
add_executable(eqn
    diacrit.c eqnbox.c funny.c io.c lex.c move.c pile.c sqrt.c
    font.c glob.c integral.c lookup.c mark.c matrix.c over.c paren.c
    shift.c size.c text.c fromto.c ${BISON_EqnParser_OUTPUTS}
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
