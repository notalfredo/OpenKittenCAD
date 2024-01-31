# Test program CMakeLists.txt

enable_testing()

add_executable(
    lexer_test 
    ./lexer_test.cpp
    /home/alfredo/repos/OpenKittenCad/src/back-end/lexer.yy.c
)

set_source_files_properties(
    /home/alfredo/repos/OpenKittenCad/src/back-end/lexer.yy.c
    PROPERTIES LANGUAGE CXX
)


# Include necessary directories
target_include_directories(lexer_test PRIVATE "${PROJECT_SOURCE_DIR}/src/back-end" "${CMAKE_CURRENT_BINARY_DIR}")

# Link against the keywords library, tau, and Flex library
target_link_libraries(lexer_test PRIVATE tau -ll)

add_test(
    lexer_test
    lexer_test
)

