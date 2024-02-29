set(VTK_DIR "/usr/local/occt_tcl_tk/vtk/lib/cmake/vtk-9.3")
set(OpenCASCADE_DIR "/usr/local/occt_install_vtk/lib/cmake/opencascade")


find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(VTK)
find_package(OpenCASCADE)


include_directories ( SYSTEM ${OpenCASCADE_INCLUDE_DIR} )


BISON_TARGET(
    MyParser
    parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c
    DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/parser.tab.h"
)

    # Flex target
FLEX_TARGET(
    MyScanner
    lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c
    DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.h"
)
 

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


include_directories(${CMAKE_CURRENT_BINARY_DIR}) # Add the current build directory to include paths

# C++ executable target
add_executable(
    driver
    main.cpp
    functions.cpp
    enumToString.cpp
    semantic.cpp
    nodeAllocFree.cpp
    symbolTable.cpp
    nodeImpl.cpp
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


# Link against the Flex library
target_include_directories(driver PUBLIC ./)


foreach (LIB ${VTK_LIBRARIES})
    target_link_libraries(driver "${LIB}")
endforeach()
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(driver "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so")
  target_link_libraries(driver "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so.7.8")
endforeach()

