cmake_minimum_required(VERSION 3.5)

project(
    OpenKittenCad
    LANGUAGES CXX
)

add_definitions(-DTEST_DIR="${PROJECT_SOURCE_DIR}")

add_subdirectory("./src/back-end/")
add_subdirectory("./src/Kitty-Script-Debugger/")
add_subdirectory("./tau/")
add_subdirectory("./tests/")
