cmake_minimum_required(VERSION 3.5)

project(
    OpenKittenCad
    LANGUAGES CXX
)

add_subdirectory("./src/back-end/")
add_subdirectory("./tau/")
add_subdirectory("./tests/")
