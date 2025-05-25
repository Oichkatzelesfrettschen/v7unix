BISON_TARGET(LexParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c COMPILE_FLAGS -y)
add_executable(lex
    lmain.c sub1.c sub2.c header.c ${BISON_LexParser_OUTPUTS}
)
target_compile_options(lex PRIVATE -Wall -Werror)
add_library(ln STATIC
    lib/allprint.c lib/main.c lib/reject.c lib/yyless.c lib/yywrap.c
)
target_compile_options(ln PRIVATE -Wall -Werror)
