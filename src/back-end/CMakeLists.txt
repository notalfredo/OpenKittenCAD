cmake_minimum_required(VERSION 3.5)

project(OpenKittenCad)

find_package(FLEX)
find_package(BISON)

#message("FLEX_FOUND         = ${FLEX_FOUND}")
#message("FLEX_EXECUTABLE    = ${FLEX_EXECUTABLE}")
#message("FLEX_VERSION       = ${FLEX_VERSION}")
#message("FLEX_LIBRARIES     = ${FLEX_LIBRARIES}")
#message("FLEX_INCLUDE_DIRS  = ${FLEX_INCLUDE_DIRS}")

#set(FLEX_lexer.yy.h_OUTPUT_HEADER "lexer.yy.h")

FLEX_TARGET(
    MyScanner
    lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c
    COMPILE_FLAGS ""
    DEFINES_FILE "lexer.yy.h"
)

add_executable(
    OpenKittenCad
    main.cpp
    lexer.l
    parser.y
    "lexer.yy.h"
)
