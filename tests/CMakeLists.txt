set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem /usr/local/flex/include/")
enable_testing()


add_executable(
    lexer_test 
    ./lexer_test.cpp
)

target_link_libraries(lexer_test PUBLIC tau)
target_link_libraries(lexer_test PUBLIC keywords)


set_target_properties(
    lexer_test
    PROPERTIES
    LINKER_LANGUAGE
    CXX
)

add_test(
    lexer_test
    lexer_test
)
