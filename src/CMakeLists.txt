add_subdirectory("./frontend/")


set(OpenCASCADE_DIR "/usr/local/occt_install_vtk/lib/cmake/opencascade")
find_package(OpenCASCADE)

add_executable(
    main
    main.cpp
)

add_dependencies(main frontEnd)

target_link_libraries(main "${CMAKE_CURRENT_BINARY_DIR}/frontend/libfrontEnd.a")


foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(main "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so")
  target_link_libraries(main "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so.7.8")
endforeach()



#cmake_minimum_required(VERSION 3.5)
#
#project(
#    occt_imgui
#    LANGUAGES CXX
#)
#
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
#
#
#set(OpenCASCADE_DIR "/usr/local/occt_install_vtk/lib/cmake/opencascade")
#
#find_package(OpenCASCADE)
#find_package(glfw3 REQUIRED)
#find_package(OpenGL REQUIRED)
#
#
#source_group ("Headers" FILES
#  GlfwOcctView.h
#  GlfwOcctWindow.h)
#source_group ("Sources" FILES
#  GlfwOcctView.cpp
#  GlfwOcctWindow.cpp
#)
#
#
## Set the output directory for the executable
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#
## Add ImGui source files
#set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
#file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp)
#file(GLOB IMGUI_BACKENDS_SOURCES ${IMGUI_DIR}/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)
#
#
#include_directories(${IMGUI_DIR} ${IMGUI_DIR}/imgui)
#
#
#set(SOURCES main.cpp)
#
#
#add_compile_options(-g -Wall -Wformat)
## Set additional libraries for Linux
#if(UNIX AND NOT APPLE)
#    set(LINUX_GL_LIBS -lGL -ldl)  # Add the -ldl flag here
#endif()
#
#
#add_executable(
#    ${PROJECT_NAME}
#    ${SOURCES}
#    GlfwOcctWindow.cpp    
#    GlfwOcctView.cpp
#    ${IMGUI_SOURCES}
#    ${IMGUI_BACKENDS_SOURCES}
#)
#
## Link libraries
#target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_LIBRARIES})
#
## Link additional libraries for Linux
#if(UNIX AND NOT APPLE)
#    target_link_libraries(${PROJECT_NAME} ${LINUX_GL_LIBS})
#endif()
#
#
#
#foreach (LIB ${OpenCASCADE_LIBRARIES})
#  target_link_libraries(${PROJECT_NAME} "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so")
#  target_link_libraries(${PROJECT_NAME} "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so.7.8")
#endforeach()
