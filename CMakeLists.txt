cmake_minimum_required(VERSION 3.5)

project(OpenKittenCad)

find_package(FLEX)
find_package(BISON)

message("${PROJECT_SOURCE_DIR}")
message("${PROJECT_SOURCE_DIR}/src/back-end/keywords.hxx")

FLEX_TARGET(
    MyScanner
    ./src/back-end/lexer.l
    "${PROJECT_SOURCE_DIR}/src/back-end/lexer.yy.c"
    COMPILE_FLAGS ""
    DEFINES_FILE "lexer.yy.h"
)

add_executable(
    OpenKittenCad
    ./src/back-end/main.cpp
    ./src/back-end/lexer.l
    "${PROJECT_SOURCE_DIR}/lexer.yy.h"
)

target_include_directories(
    OpenKittenCad
    PUBLIC
    "${PROJECT_SOURCE_DIR}/src/back-end/keywords.hxx"
)

enable_testing()
add_executable(
    lexer_tests
    src/back-end-test/lexer_test.c
)
target_include_directories(
    lexer_tests
    PUBLIC
    .
)

add_test(
    lexer_tests
    lexer_tests
)
