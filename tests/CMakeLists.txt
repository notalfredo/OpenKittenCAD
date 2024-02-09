# Test program CMakeLists.txt

enable_testing()

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(
    MyParser
    /home/alfredo/repos/OpenKittenCad/src/back-end/parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c
    DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/parser.tab.h"
)

    # Flex target
FLEX_TARGET(
    MyScanner
    /home/alfredo/repos/OpenKittenCad/src/back-end/lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c
    DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.h"
)
 

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


include_directories(
    /home/alfredo/repos/OpenKittenCad/src/back-end/
) 

add_executable(
    lexer_test 
    ./lexer_test.cpp
    /home/alfredo/repos/OpenKittenCad/src/back-end/nodeImpl.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)

add_executable(
    parser_test 
    ./parser_test.cpp
    /home/alfredo/repos/OpenKittenCad/src/back-end/nodeImpl.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)


# Set the language for lex.yy.c to C++ 
# NOTE WE ARE NOT USING C++ Flex scanner
set_source_files_properties(
    parser.tab.c
    PROPERTIES LANGUAGE CXX
)

set_source_files_properties(
    lexer.yy.c
    PROPERTIES LANGUAGE CXX
)

target_include_directories(lexer_test
    PUBLIC
    ./
)
target_include_directories(lexer_test
    PUBLIC
    /home/alfredo/repos/OpenKittenCad/src/back-end/node.hxx
)
target_link_libraries(lexer_test PRIVATE tau)
target_link_libraries(lexer_test PRIVATE -ly -ll)


target_include_directories(parser_test
    PUBLIC
    ./
)
target_include_directories(parser_test
    PUBLIC
    /home/alfredo/repos/OpenKittenCad/src/back-end/node.hxx
)


target_link_libraries(
    parser_test
    PUBLIC
    tau
)
target_link_libraries(
    parser_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/build/src/tree-dump/libTreeDumper.a
)
target_link_libraries(
    parser_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/build/src/tree-dump/libTreeDumperHelper.a
)

add_test(
    lexer_test
    lexer_test
)


add_test(
    parser_test
    parser_test
)


