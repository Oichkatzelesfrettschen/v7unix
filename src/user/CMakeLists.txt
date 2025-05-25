file(GLOB LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c")
add_library(exo_user STATIC ${LIB_SRC})
target_include_directories(exo_user PUBLIC ${CMAKE_SOURCE_DIR}/src/kernel)
target_include_directories(exo_user PUBLIC ${CMAKE_SOURCE_DIR}/v7/usr/sys/h)
