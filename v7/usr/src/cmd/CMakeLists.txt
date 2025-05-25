add_subdirectory(awk)
add_subdirectory(eqn)
add_subdirectory(neqn)
add_subdirectory(lex)
add_subdirectory(m4)
add_subdirectory(make)

# Single file grammars
bison_target(BcParser ../bc.y ${CMAKE_CURRENT_BINARY_DIR}/bc.c COMPILE_FLAGS -y)
add_executable(bc ${BISON_BcParser_OUTPUTS})

bison_target(EgrepParser ../egrep.y ${CMAKE_CURRENT_BINARY_DIR}/egrep.c COMPILE_FLAGS -y)
add_executable(egrep ${BISON_EgrepParser_OUTPUTS})

bison_target(ExprParser ../expr.y ${CMAKE_CURRENT_BINARY_DIR}/expr.c COMPILE_FLAGS -y)
add_executable(expr ${BISON_ExprParser_OUTPUTS})
