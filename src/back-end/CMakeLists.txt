add_library(keywords keywords.hxx)
set_target_properties(keywords PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(keywords PUBLIC ./)

# Flex target
find_package(FLEX REQUIRED)
flex_target(
    MyScanner
    lexer.l
    lexer.yy.c
)

# C++ executable target
add_executable(
    driver
    main.cpp
    ${FLEX_MyScanner_OUTPUTS}
)


# Set the language for lex.yy.c to C++ 
# NOTE WE ARE NOT USING C++ Flex scanner
set_source_files_properties(
    lexer.yy.c
    PROPERTIES LANGUAGE CXX
)


# Link against the Flex library
target_link_libraries(driver PUBLIC keywords)
target_link_libraries(driver PRIVATE -ll)
