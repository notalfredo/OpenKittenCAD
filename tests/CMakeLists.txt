# Test program CMakeLists.txt

enable_testing()

add_executable(
    lexer_test 
    ./lexer_test.cpp
    /home/alfredo/repos/OpenKittenCad/src/back-end/lexer.yy.c
)

# Include necessary directories
target_include_directories(lexer_test PRIVATE "${PROJECT_SOURCE_DIR}/src/back-end" "${CMAKE_CURRENT_BINARY_DIR}")

# Link against the keywords library, tau, and Flex library
target_link_libraries(lexer_test PRIVATE keywords tau -ll)

add_test(
    lexer_test
    lexer_test
)

