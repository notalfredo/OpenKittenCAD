find_package(FLEX REQUIRED)

add_library(keywords keywords.hxx)
set_target_properties(keywords PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(keywords PUBLIC ./)

FLEX_TARGET(
    Lexer
    lexer.l
    "./lexer.yy.c"
    COMPILE_FLAGS ""
    DEFINES_FILE "lexer.yy.h"
)

#FLEX_${Name}_DEFINED - true is the macro ran successfully
#FLEX_${Name}_OUTPUTS - the source file generated 
                            #by the custom rule, an alias for FlexOutput
#FLEX_${Name}_INPUT - the flex source file, an alias for ${FlexInput}
#FLEX_${Name}_OUTPUT_HEADER - the header flex output, if any.

message("1: ${FLEX_Lexer_DEFINED}")
message("2: ${FLEX_Lexer_OUTPUTS}")
message("3: ${FLEX_Lexer_INPUT}")
message("4: ${FLEX_Lexer_OUTPUT_HEADER}")

add_library(
    lex
    lexer.yy.c
)
set_target_properties(lex PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(lex PUBLIC keywords)


add_library(
    main
    main.cpp
)
set_target_properties(lex PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(main PUBLIC lex)

add_executable(
    main_exe
    main 
)
