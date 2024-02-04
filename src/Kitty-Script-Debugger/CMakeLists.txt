include_directories(
    /home/alfredo/repos/OpenKittenCad/src/back-end/
) 

add_library(
    helper
    DebuggerHelperFunc.cpp
    DebuggerHelperFunc.hxx
)

set_target_properties(helper PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(helper PUBLIC ./)

add_library(
    debugger     
    ./KittyScriptDebugger.cpp
)

# Link against the Flex library
target_include_directories(debugger
    PUBLIC
    ./
)
target_include_directories(debugger
    PUBLIC
    /home/alfredo/repos/OpenKittenCad/src/back-end/node.hxx
)

target_link_libraries(
    debugger
    helper
)
