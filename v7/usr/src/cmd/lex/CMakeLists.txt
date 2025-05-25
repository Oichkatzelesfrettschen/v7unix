BISON_TARGET(LexParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c COMPILE_FLAGS -y)
add_executable(lex
    lmain.c sub1.c sub2.c header.c ${BISON_LexParser_OUTPUTS}
)
add_library(ln STATIC
    lib/allprint.c lib/main.c lib/reject.c lib/yyless.c lib/yywrap.c
)
