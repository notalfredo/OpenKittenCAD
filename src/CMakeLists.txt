add_subdirectory("./frontend/")


find_package(OpenCASCADE REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets Xml REQUIRED)


set(SRC_FOLDER "${PROJECT_SOURCE_DIR}/src/")
set(CURR_PROJECT_SRC_FOLDER "${PROJECT_SOURCE_DIR}/src/src/")
set(MORE_CODE "${PROJECT_SOURCE_DIR}/src/common")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(RESOURCE_FILES
    "${CURR_PROJECT_SRC_FOLDER}/OCCTOverview.qrc"
    "${MORE_CODE}/Samples.qrc"
)

include_directories(${CURR_PROJECT_SRC_FOLDER})
include_directories(${MORE_CODE})

file(GLOB OCCTOVRW_SRC "${MORE_CODE}/*.cxx")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(occtovrw ${OCCTOVRW_SRC})
target_link_libraries(occtovrw Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml)

foreach (LIB ${OpenCASCADE_LIBRARIES})
    target_link_libraries(occtovrw "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so")
    target_link_libraries(occtovrw "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so.7.8")
endforeach()


add_library(qt_src_sources
    "${CURR_PROJECT_SRC_FOLDER}/ApplicationCommon.cxx"
    "${CURR_PROJECT_SRC_FOLDER}/DocumentCommon.cxx"
    "${CURR_PROJECT_SRC_FOLDER}/GeomWidget.cxx"
    "${CURR_PROJECT_SRC_FOLDER}/OcctHighlighter.cxx"
    "${CURR_PROJECT_SRC_FOLDER}/OcctWindow.cxx"
    "${CURR_PROJECT_SRC_FOLDER}/TranslateDialog.cxx"
    "${CURR_PROJECT_SRC_FOLDER}/Transparency.cxx"
    "${CURR_PROJECT_SRC_FOLDER}/View.cxx"
)

target_link_libraries(qt_src_sources
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
    occtovrw
)

target_include_directories(qt_src_sources PRIVATE
    ${CURR_PROJECT_SRC_FOLDER}
    ${MORE_CODE}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

foreach (LIB ${OpenCASCADE_LIBRARIES})
    target_link_libraries(qt_src_sources "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so")
    target_link_libraries(qt_src_sources "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so.7.8")
endforeach()


add_executable(main "${SRC_FOLDER}/Main.cxx")

add_dependencies(main frontEnd)

target_sources(main PRIVATE ${RESOURCE_FILES})
target_link_libraries(main qt_src_sources)

target_link_libraries(main "${CMAKE_CURRENT_BINARY_DIR}/frontend/libfrontEnd.a")



foreach (LIB ${OpenCASCADE_LIBRARIES})
    target_link_libraries(main "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so")
    target_link_libraries(main "${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so.7.8")
endforeach()

