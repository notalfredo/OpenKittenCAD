# Test program CMakeLists.txt
enable_testing()


find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(VTK)
find_package(OpenCASCADE)


include_directories ( SYSTEM ${OpenCASCADE_INCLUDE_DIR} )


BISON_TARGET(
    MyParser 
    ${PROJECT_SOURCE_DIR}/src/frontend/parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c
    COMPILE_FLAGS "--report=itemset --graph --report=lookaheads"
    DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/parser.tab.h"
)

FLEX_TARGET(
    MyScanner
    ${PROJECT_SOURCE_DIR}/src/frontend/lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c
    DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.h"
)
 
include_directories("${PROJECT_SOURCE_DIR}/src/frontend/") 

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


add_library(CommonSources
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeAllocFree.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeImpl.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)

set(COMMON_INCLUDE_DIRECTORIES
    ./
    ${PROJECT_SOURCE_DIR}/src/frontend/node.hxx
)


add_executable(lexer_test ./lexer_test.cpp)
add_executable(parser_test
    ./parser_test.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/enumToString.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeImpl.cpp
)
add_executable(
    symbol_table_test 
    ./symbol_table_test.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/enumToString.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeAllocFree.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/symbolTable.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeImpl.cpp
)
add_executable(semantic_test
    ./semantic_test.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/validateFunc.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/functions.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeAllocFree.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/symbolTable.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeImpl.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/enumToString.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/semantic.cpp
)
add_executable(vtk_test
    ./vtk_test.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/validateFunc.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/functions.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeAllocFree.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/symbolTable.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeImpl.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/enumToString.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/semantic.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/frontEndEntryPoint.cpp
)
add_executable(functions_test
    ./functions_test.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/validateFunc.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/enumToString.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeImpl.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/nodeAllocFree.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/functions.cpp
)


target_link_libraries(parser_test
    PUBLIC 
    CommonSources
    tau
)
target_link_libraries(lexer_test PUBLIC CommonSources tau)
target_link_libraries(symbol_table_test PUBLIC tau)
target_link_libraries(semantic_test
    CommonSources
    tau
)
target_link_libraries(vtk_test
    CommonSources
    tau
)
target_link_libraries(functions_test tau)


foreach (LIB ${VTK_LIBRARIES})
    target_link_libraries(vtk_test "${LIB}")
endforeach()
foreach (LIB ${OpenCASCADE_LIBRARIES})
    target_link_libraries(semantic_test "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so")
    target_link_libraries(semantic_test "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so.7.8")

    target_link_libraries(vtk_test "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so")
    target_link_libraries(vtk_test "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so.7.8")

    target_link_libraries(functions_test "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so")
    target_link_libraries(functions_test "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so.7.8")
endforeach()




# Set the language for lex.yy.c to C++ 
# NOTE WE ARE NOT USING C++ Flex scanner
set_source_files_properties(parser.tab.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(lexer.yy.c PROPERTIES LANGUAGE CXX)


target_compile_options(lexer_test PRIVATE -g)
target_compile_options(parser_test PRIVATE -g)
target_compile_options(symbol_table_test PRIVATE -g)
target_compile_options(semantic_test PRIVATE -g)
target_compile_options(vtk_test PRIVATE -g)
target_compile_options(functions_test PRIVATE -g)


target_include_directories(lexer_test PUBLIC ${COMMON_INCLUDE_DIRECTORIES})
target_include_directories(parser_test PUBLIC ${COMMON_INCLUDE_DIRECTORIES})
target_include_directories(symbol_table_test PUBLIC ${COMMON_INCLUDE_DIRECTORIES})
target_include_directories(semantic_test PUBLIC ${COMMON_INCLUDE_DIRECTORIES})
target_include_directories(vtk_test PUBLIC ${COMMON_INCLUDE_DIRECTORIES})
target_include_directories(functions_test PUBLIC ${COMMON_INCLUDE_DIRECTORIES})


add_test(lexer_test lexer_test)
add_test(parser_test parser_test)
add_test(symbol_table_test symbol_table_test)
add_test(functions_test functions_test)
