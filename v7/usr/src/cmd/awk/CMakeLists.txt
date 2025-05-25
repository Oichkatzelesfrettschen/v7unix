find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(AwkParser awk.g.y ${CMAKE_CURRENT_BINARY_DIR}/awk.g.c
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/y.tab.h
             COMPILE_FLAGS -y -d)
FLEX_TARGET(AwkLexer awk.lx.l ${CMAKE_CURRENT_BINARY_DIR}/awk.lx.c)
ADD_FLEX_BISON_DEPENDENCY(AwkLexer AwkParser)

add_executable(awk
    ${BISON_AwkParser_OUTPUTS}
    ${FLEX_AwkLexer_OUTPUTS}
    b.c lib.c main.c parse.c proc.c run.c token.c tran.c
)
target_compile_options(awk PRIVATE -Wall -Werror)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
