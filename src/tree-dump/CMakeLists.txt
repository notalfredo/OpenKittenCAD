include_directories(
    /home/alfredo/repos/OpenKittenCad/src/back-end/
) 

add_library(
    TreeDumperHelper
    TreeDumpHelper.cpp
    TreeDumpHelper.hxx
)

set_target_properties(
    TreeDumperHelper
    PROPERTIES
    LINKER_LANGUAGE
    CXX
)

target_include_directories(
    TreeDumperHelper
    PUBLIC
    ./
)

add_library(
    TreeDumper
    ./TreeDump.cpp
)

# Link against the Flex library
target_include_directories(
    TreeDumper
    PUBLIC
    ./
)

target_include_directories(
    TreeDumper
    PUBLIC
    /home/alfredo/repos/OpenKittenCad/src/back-end/node.hxx
)

target_link_libraries(
    TreeDumper
    TreeDumpHelper
)
