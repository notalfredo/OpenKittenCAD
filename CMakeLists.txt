cmake_minimum_required(VERSION 3.5)

project(
    OpenKittenCad
    LANGUAGES CXX
)

add_definitions(-DPROJ_DIR="${PROJECT_SOURCE_DIR}")

option(BUILD_TEST "Enable or disable testing" OFF)
option(BUILD_MAIN "Enable or disable testing" OFF)


if(BUILD_MAIN)
    add_subdirectory("./src/back-end/")
endif()

if(BUILD_TEST)
    add_subdirectory("./tau/")
    add_subdirectory("./tests/")
endif()
