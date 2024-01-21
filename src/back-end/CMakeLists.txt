set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem /usr/local/flex/include/")

find_package(FLEX REQUIRED)

add_library(keywords keywords.hxx)
set_target_properties(keywords PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(keywords PUBLIC ./)

set(FLEX_LIBRARIES "/usr/local/flex/lib/libfl.so")

FLEX_TARGET(
    gen_lexer
    lexer.l
    lexer.yy.cc
)

add_executable(
   main
   main.cpp
   ${FLEX_gen_lexer_OUTPUTS}
)


message("FLEX_gen_lexer_outputs ${FLEX_gen_lexer_OUTPUTS}")
message("FLEX_LIBRARIES ${FLEX_LIBRARIES}")

message("=============================")

target_link_libraries(main PUBLIC keywords)
target_link_libraries(main PUBLIC ${FLEX_LIBRARIES})
