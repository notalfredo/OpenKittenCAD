add_library(
    tau
    tau.h
)

target_include_directories(tau PUBLIC .)
set_target_properties(tau PROPERTIES LINKER_LANGUAGE CXX)
