cmake_minimum_required(VERSION 3.5)

project(
    OpenKittenCad
    LANGUAGES CXX
)

add_definitions(-DPROJ_DIR="${PROJECT_SOURCE_DIR}")

option(BUILD_TEST "Enable or disable testing" OFF)
option(BUILD_MAIN "Enable or disable testing" OFF)

# Allow users to specify VTK_DIR and OpenCASCADE_DIR
set(VTK_DIR_ARG "" CACHE PATH "Path to VTK directory")
set(OpenCASCADE_DIR_ARG "" CACHE PATH "Path to OpenCASCADE directory")

if(VTK_DIR_ARG)
    set(VTK_DIR "${VTK_DIR_ARG}")
endif()

if(OpenCASCADE_DIR_ARG)
    set(OpenCASCADE_DIR "${OpenCASCADE_DIR_ARG}")
endif()

if(BUILD_MAIN)
    add_subdirectory("./src/back-end/")
endif()

if(BUILD_TEST)
    add_subdirectory("./tau/")
    add_subdirectory("./tests/")
endif()

